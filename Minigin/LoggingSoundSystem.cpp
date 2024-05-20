#include "SoundSystem.h"
#include <chrono>
#include <iostream>


namespace Jotar
{


    void LoggingSoundSystem::Update()
    {
        m_SoundSystem->Update();
    }

    void LoggingSoundSystem::Play(const sound_id id, const int volume)
    {
        auto start = std::chrono::high_resolution_clock::now();

        m_SoundSystem->Play(id, volume);

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "playing " << id << " at volume " << volume << '\n';
        std::cout << "playing the sound took: " << elapsed << "ms\n";
    }

    void LoggingSoundSystem::LoadSound(sound_id id)
    {
        std::cout << "Loading audio...\n";
        m_SoundSystem->LoadSound(id);
    }

    void LoggingSoundSystem::AddSound(const char* path, sound_id id)
    {
        m_SoundSystem->AddSound(path, id);
    }

    void LoggingSoundSystem::PlayMusic(const char* path)
    {
        std::cout << "Playing this background Music: " << path << "\n";
        m_SoundSystem->PlayMusic(path);
    }

    void LoggingSoundSystem::SetMusicVolume(int volume)
    {
        std::cout << "Changing music Volume to:" << volume << "\n";

        m_SoundSystem->SetMusicVolume(volume);
    }

    void LoggingSoundSystem::SetSoundEffectsVolume(int volume)
    {
        std::cout << "Changing Sound effects Volume to:" << volume << "\n";

        m_SoundSystem->SetSoundEffectsVolume(volume);
    }

    void LoggingSoundSystem::MuteSound()
    {
        std::cout << "Toggle Mute" << "\n";
        m_SoundSystem->MuteSound();
    }

    int LoggingSoundSystem::GetMusicVolume() const
    {
        int volume = m_SoundSystem->GetMusicVolume();
        std::cout << "Music volume is: " << volume << "\n";
        return volume;
    }

    int LoggingSoundSystem::GetSoundEffectsVolume() const
    {
        int volume = m_SoundSystem->GetSoundEffectsVolume();
        std::cout << "Sound Effects volume is: " << volume << "\n";
        return volume;
    }
}