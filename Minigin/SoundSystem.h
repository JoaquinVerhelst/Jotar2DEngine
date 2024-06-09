#pragma once
#include <memory>
#include <string>

namespace Jotar
{
	using sound_id = int;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual void Update() = 0;
		virtual void Play(const sound_id id, const int volume = -1) = 0;
		virtual void Play(const std::string& name, const int volume = -1) = 0;
		virtual void LoadSound(sound_id id) = 0;
		virtual void AddSound(const char* path, sound_id id = -1, const std::string& name = "") = 0;
		virtual void AddMusic(const char* path, const std::string& name = "") = 0;
		virtual void StopMusic() = 0;
		virtual void PlayMusic(const std::string& name) = 0;
		virtual void SetMusicVolume(int volume) = 0;
		virtual void SetSoundEffectsVolume(int volume) = 0;
		virtual void MuteSound() = 0;

		virtual int GetMusicVolume() const = 0;
		virtual int GetSoundEffectsVolume() const = 0;

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
		void Play(const std::string& name, const int volume = -1) override;
		void LoadSound(sound_id id) override;
		void AddSound(const char* path, sound_id id = -1, const std::string& name = "") override;
		void AddMusic(const char* path, const std::string& name = "") override;
		void StopMusic() override;
		void PlayMusic(const std::string& name) override;
		void SetMusicVolume(int volume);
		void SetSoundEffectsVolume(int volume) override;
		void MuteSound() override;

		int GetMusicVolume() const override;
		int GetSoundEffectsVolume() const override;
	};


	class LoggingSoundSystem final : public SoundSystem {
		std::unique_ptr<SoundSystem> m_SoundSystem;
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : m_SoundSystem(std::move(ss)) {}
		~LoggingSoundSystem() = default;

		void Update() override;
		void Play(const sound_id id, const int volume = -1) override;
		void Play(const std::string& name, const int volume = -1) override;
		void LoadSound(sound_id id) override;
		void AddSound(const char* path, sound_id id = -1, const std::string& name = "") override;
		void AddMusic(const char* path, const std::string& name = "") override;
		void PlayMusic(const std::string& name) override;
		void StopMusic() override;
		void SetMusicVolume(int volume) override;
		void SetSoundEffectsVolume(int volume) override;
		void MuteSound() override;

		int GetMusicVolume() const override;
		int GetSoundEffectsVolume( ) const override;
	};


	class NullSoundSystem final : public SoundSystem
	{
	public:
		void Update() override {}
		void Play(const sound_id , const int = -1) override {}
		void Play(const std::string& , const int  = -1) override {}
		void LoadSound(sound_id ) override {}
		void AddSound(const char* , sound_id  = -1, const std::string&  = "") override {}
		void AddMusic(const char* , const std::string&  = "") override {}
		void PlayMusic(const std::string&) override {}
		void StopMusic() override {}
		void SetMusicVolume(int ) override {}
		void SetSoundEffectsVolume(int ) override {}
		void MuteSound() override {}

		int GetMusicVolume() const override { return 0; };
		int GetSoundEffectsVolume() const override { return 0; };
	};


}