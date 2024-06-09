#include "SoundSystem.h"
#include "SDL_mixer.h"
#include <SDL.h>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <future>
#include <thread>
#include <iostream>

namespace Jotar
{
    class SDL_SoundSystem::SDL_SoundSystemImpl
    {
        struct PlayMessage
        {
            sound_id id;
            int volume;
        };

        struct AudioClip
        {
            sound_id id;
            Mix_Chunk* audio;
            const char* pathFile;
            std::string audioName;
        };

        struct MusicAudio
        {
            sound_id id;
            Mix_Music* audio;
            const char* pathFile;
            std::string audioName;
        };


    public:

        SDL_SoundSystemImpl();
        ~SDL_SoundSystemImpl();

        void Update();
        void Play(const sound_id id, const int volume);
        void Play(const std::string& name, const int volume = -1);
        void LoadSound(const sound_id id);
        void AddSound(const char* path, const sound_id id = -1, const std::string& name = "");
        void AddMusic(const char* path, const std::string& name = "");
        void PlayMusic(const std::string& name);
        void StopMusic();
        void SetMusicVolume(int volume);
        void SetSoundEffectsVolume(int volume);

        int GetMusicVolume() const;
        int GetSoundEffectsVolume() const;


        void MuteSound();
        void EnqueueTask(std::function<void()> task);

    private:
        void LoadAndPlaySound(AudioClip* audioClip, int volume);
        void PlaySound(AudioClip* audioClip, int volume);
        void WorkerThread();

        // Event Queue
        static const int MAX_PENDING = 16;
        static PlayMessage m_Pending[MAX_PENDING];
        static int m_NumPending;

        // Audio
        std::vector<AudioClip*> m_AudioClips;
        std::vector<MusicAudio*> m_BackgroundMusic;

        bool m_SoundMuted;
        int m_MusicVolume;
        int m_SoundEffectVolume;

        //Threads
        std::thread m_thread;
        std::queue<std::function<void()>> m_taskQueue;

        //synchronizing access to the sound task queue 
        std::mutex m_queueMutex;

        //used for thread safety during sound loading in the LoadSound function.
        std::mutex m_SoundMutex;
        std::condition_variable m_taskCondition;
        bool m_Stop;
    };

    SDL_SoundSystem::SDL_SoundSystemImpl::PlayMessage SDL_SoundSystem::SDL_SoundSystemImpl::m_Pending[MAX_PENDING];
    int SDL_SoundSystem::SDL_SoundSystemImpl::m_NumPending = 0;

    // --------------------------------------------------------------------------------------
    // SDL Sound pImpl System functions------------------------------------------------------
    // --------------------------------------------------------------------------------------


    SDL_SoundSystem::SDL_SoundSystemImpl::SDL_SoundSystemImpl()
        : m_Stop{false}
        , m_SoundMuted{ false }
        , m_SoundEffectVolume{ MIX_MAX_VOLUME }
        , m_MusicVolume { MIX_MAX_VOLUME / 2 }
        , m_BackgroundMusic{}
    {
        // Initialize SDL and SDL_mixer
        SDL_Init(SDL_INIT_AUDIO);
        Mix_Init(MIX_INIT_MP3);

        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
        m_thread = std::thread([this] { WorkerThread(); });
    }

    SDL_SoundSystem::SDL_SoundSystemImpl::~SDL_SoundSystemImpl()
    {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_Stop = true;
            m_taskCondition.notify_one();
        } // Release the lock here before joining the thread

        m_thread.join();

        for (auto* audio : m_AudioClips) {
            Mix_FreeChunk(audio->audio);
            delete audio;
        }

        Mix_HaltMusic();
        for (auto* music : m_BackgroundMusic) {
            Mix_FreeMusic(music->audio);
            delete music;
        }
        Mix_CloseAudio();
    }


    void SDL_SoundSystem::SDL_SoundSystemImpl::Update()
    {
        for (int i = 0; i <= m_NumPending - 1; ++i)
        {
            AudioClip* audioClip = m_AudioClips[m_Pending[i].id];

            if (audioClip->audio == nullptr)
            {
                EnqueueTask([this, audioClip, volume = m_Pending[i].volume]() {
                    LoadAndPlaySound(audioClip, volume);
                    });
            }
            else
            {
                EnqueueTask([this, audioClip, volume = m_Pending[i].volume]() {
                    PlaySound(audioClip, volume);
                    });
            }
        }

        m_NumPending = 0;
    }


    void SDL_SoundSystem::SDL_SoundSystemImpl::WorkerThread()
    {
        while (true)
        {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_taskCondition.wait(lock, [this] { return m_Stop || !m_taskQueue.empty(); });

                if (m_Stop && m_taskQueue.empty())
                    break;

                task = std::move(m_taskQueue.front());
                m_taskQueue.pop();
            }

            task();
        }
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::EnqueueTask(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_taskQueue.emplace(std::move(task));
        }
        m_taskCondition.notify_one();
    }


    void SDL_SoundSystem::SDL_SoundSystemImpl::Play(const sound_id id, const int volume)
    {
        if (m_NumPending >= MAX_PENDING)
        {
            std::cout << "Maximum pending sounds. Cannot play ID: " << id << '\n';
            return;
        }
        
        if (volume == -1.f)
            m_Pending[m_NumPending].volume = m_SoundEffectVolume;
        else
            m_Pending[m_NumPending].volume = volume;

        m_Pending[m_NumPending].id = id;
        ++m_NumPending;
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::Play(const std::string& name, const int volume)
    {
        auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&name](const AudioClip* ptr)
            {
                return ptr->audioName == name;
            });

        if (it != m_AudioClips.end())
        {
            Play((*it)->id, volume);
        }
        else
        {
            std::cout << "Audio clip with name " << name << " not found.\n";
        }
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::LoadAndPlaySound(AudioClip* audioClip, int volume)
    {
        LoadSound(audioClip->id);
        PlaySound(audioClip, volume);
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::LoadSound(const sound_id id)
    {
        auto* audioclip = m_AudioClips[id];

        Mix_Chunk* sound = Mix_LoadWAV(audioclip->pathFile);
        if (!sound) {
            std::cerr << "Error: Could not load sound ID: " << audioclip->id << " with the given path: " << audioclip->pathFile << std::endl;
            return;
        }

        std::lock_guard<std::mutex> lock(m_SoundMutex);
        audioclip->audio = sound;
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::PlaySound(AudioClip* audioClip, int volume)
    {

        std::lock_guard<std::mutex> lock(m_SoundMutex);

        Mix_VolumeChunk(audioClip->audio, static_cast<int>(volume * MIX_MAX_VOLUME));

        if (Mix_PlayChannel(-1, audioClip->audio, 0) == -1)
        {
            std::cout << "Could not play sound" << '\n';
            return;
        }
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::AddSound(const char* path, sound_id id, const std::string& name)
    {
        AudioClip* audioClip = new AudioClip();
        audioClip->audio = nullptr;
        audioClip->pathFile = path;
        audioClip->audioName = name;

        if (id == -1)
        {
            audioClip->id = static_cast<int>(m_AudioClips.size());
            m_AudioClips.emplace_back(audioClip);
        }
        else if (id < m_AudioClips.size())
        {
            audioClip->id = id;
            delete m_AudioClips[id];
            m_AudioClips[id] = audioClip;
        }
        else
        {
            std::cout << "Failed to add audio to ID: " << id << "with the given path: " << path << '\n';
        }
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::AddMusic(const char* path, const std::string& name)
    {
        MusicAudio* musicAudio = new MusicAudio();
        musicAudio->audio = nullptr;
        musicAudio->pathFile = path;
        musicAudio->audioName = name;

        musicAudio->id = static_cast<int>(m_BackgroundMusic.size());

        m_BackgroundMusic.emplace_back(musicAudio);
        
    }


    void SDL_SoundSystem::SDL_SoundSystemImpl::PlayMusic(const std::string& name)
    {

        auto it = std::find_if(m_BackgroundMusic.begin(), m_BackgroundMusic.end(), [&name](const MusicAudio* ptr)
            {
                return ptr->audioName == name;
            });


        if (it != m_BackgroundMusic.end())
        {
            int ID = (*it)->id;
            if (m_BackgroundMusic[ID]->audio == nullptr)
            {
                m_BackgroundMusic[ID]->audio = Mix_LoadMUS(m_BackgroundMusic[ID]->pathFile);
            }

            if (!m_BackgroundMusic[ID]->audio) {
                printf("Mix_LoadMUS failed: %s\n", Mix_GetError());
                return;
            }

            Mix_PlayMusic(m_BackgroundMusic[ID]->audio, -1);
        }
        else
        {
            std::cout << "Music Adio with name " << name << " not found.\n";
        }
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::StopMusic()
    {
        Mix_HaltMusic();
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::SetMusicVolume(int volume)
    {
        if (volume < 0) {
            volume = 0;
        }
        else if (volume > MIX_MAX_VOLUME) {
            volume = MIX_MAX_VOLUME;
        }

        m_MusicVolume = volume;
        Mix_VolumeMusic(m_MusicVolume);
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::SetSoundEffectsVolume(int volume)
    {
        if (volume < 0) {
            volume = 0;
        }
        else if (volume > MIX_MAX_VOLUME) {
            volume = MIX_MAX_VOLUME;
        }

        m_SoundEffectVolume = volume;
        Mix_VolumeMusic(m_SoundEffectVolume);
    }

    int SDL_SoundSystem::SDL_SoundSystemImpl::GetMusicVolume() const
    {
        return m_MusicVolume;
    }

    int SDL_SoundSystem::SDL_SoundSystemImpl::GetSoundEffectsVolume() const
    {
        return m_SoundEffectVolume;
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::MuteSound()
    {
        if (!m_SoundMuted)
        {
            Mix_Volume(-1, 0);
            Mix_VolumeMusic(0);
            m_SoundMuted = true;
        }
        else
        {
            Mix_Volume(-1, m_SoundEffectVolume);
            Mix_VolumeMusic(m_MusicVolume);
            m_SoundMuted = false;
        }
    }



    // -----------------------------------------------------------------------
    // SDL Sound System ------------------------------------------------------
    // -----------------------------------------------------------------------


    SDL_SoundSystem::SDL_SoundSystem()
    {
        pImpl = new SDL_SoundSystemImpl();
    }

    SDL_SoundSystem::~SDL_SoundSystem()
    {
        delete pImpl;
    }

    void SDL_SoundSystem::Update()
    {
        pImpl->Update();
    }

    void SDL_SoundSystem::Play(const sound_id id, const int volume)
    {
        pImpl->Play(id, volume);
    }

    void SDL_SoundSystem::Play(const std::string& name, const int volume)
    {
        pImpl->Play(name, volume);
    }

    void SDL_SoundSystem::LoadSound(sound_id id)
    {
        pImpl->LoadSound(id);
    }


    void SDL_SoundSystem::AddSound(const char* path, sound_id id, const std::string& name)
    {
        pImpl->AddSound(path, id, name);
    }

    void SDL_SoundSystem::AddMusic(const char* path, const std::string& name)
    {
        pImpl->AddMusic(path, name);
    }

    void SDL_SoundSystem::PlayMusic(const std::string& name)
    {
        pImpl->PlayMusic(name);
    }

    void SDL_SoundSystem::SetMusicVolume(int volume)
    {
        pImpl->SetMusicVolume(volume);
    }

    void SDL_SoundSystem::SetSoundEffectsVolume(int volume)
    {
        pImpl->SetSoundEffectsVolume(volume);
    }

    void SDL_SoundSystem::MuteSound()
    {
        pImpl->MuteSound();
    }


    int SDL_SoundSystem::GetMusicVolume() const
    {
        return pImpl->GetMusicVolume();
    }

    int SDL_SoundSystem::GetSoundEffectsVolume() const
    {
       return pImpl->GetSoundEffectsVolume();
    }
    void SDL_SoundSystem::StopMusic()
    {
        pImpl->StopMusic();
    }
}