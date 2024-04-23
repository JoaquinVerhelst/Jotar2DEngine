#pragma once
#include <memory>


namespace Jotar
{
	using sound_id = size_t;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual void Update() = 0;
		virtual void Play(const sound_id id, const int volume = -1) = 0;
		virtual void LoadSound(sound_id id) = 0;
		virtual void AddSound(const char* path, sound_id id = -1) = 0;
		virtual void PlayMusic(const char* path) = 0;
		virtual void SetMusicVolume(int volume) = 0;
		virtual void SetSoundEffectsVolume(int volume) = 0;
		virtual void MuteSound() = 0;
	};


	class SDL_SoundSystem final : public SoundSystem
	{
		class SDL_SoundSystemImpl;
		SDL_SoundSystemImpl* pImpl;
	public:

		SDL_SoundSystem();
		~SDL_SoundSystem();

		void Update() override;
		void Play(const sound_id id, const int volume = -1) override;
		void LoadSound(sound_id id) override;
		void AddSound(const char* path, sound_id id = -1) override;
		void PlayMusic(const char* path) override;
		void SetMusicVolume(int volume);
		void SetSoundEffectsVolume(int volume) override;
		void MuteSound() override;
	};


	class LoggingSoundSystem final : public SoundSystem {
		std::unique_ptr<SoundSystem> m_SoundSystem;
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : m_SoundSystem(std::move(ss)) {}
		~LoggingSoundSystem() = default;

		void Update() override;
		void Play(const sound_id id, const int volume = -1) override;
		void LoadSound(sound_id id) override;
		void AddSound(const char* path, sound_id id = -1) override;
		void PlayMusic(const char* path) override;
		void SetMusicVolume(int volume) override;
		void SetSoundEffectsVolume(int volume) override;
		void MuteSound() override;

	};


	class NullSoundSystem final : public SoundSystem
	{
	public:
		virtual void Update() override {}
		virtual void Play(const sound_id , const int = -1) override {}
		virtual void LoadSound(sound_id ) override {}
		virtual void AddSound(const char* , sound_id = -1) override {}
		virtual void PlayMusic(const char* ) override {}
		virtual void SetMusicVolume(int ) override {}
		virtual void SetSoundEffectsVolume(int ) override {}
		virtual void MuteSound() override {}
	};


}