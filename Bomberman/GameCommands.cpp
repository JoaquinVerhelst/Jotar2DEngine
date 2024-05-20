#include "GameCommands.h"

#include "SoundServiceLocator.h"
#include "SoundSystem.h"
#include "GameManager.h"

#include <iostream>
#include "SDL.h"

void Jotar::SkipLevelCommand::Execute()
{
	std::cout << "Next Level" << '\n';
	GameManager::GetInstance().NextLevel();
}

void Jotar::MuteSoundCommand::Execute()
{
	auto value = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2);
		std::cout << value << '\n';

	//SoundServiceLocator::GetSoundSystem().MuteSound();
}

Jotar::ChangeSoundEffectVolume::ChangeSoundEffectVolume(int volumeStepSize)
	: m_VolumeStepSize{ volumeStepSize } {}

void Jotar::ChangeSoundEffectVolume::Execute()
{
	int totalVolume = SoundServiceLocator::GetSoundSystem().GetSoundEffectsVolume() + m_VolumeStepSize;
	SoundServiceLocator::GetSoundSystem().SetSoundEffectsVolume(totalVolume);
}

Jotar::ChangeMusicVolume::ChangeMusicVolume(int volumeStepSize)
	: m_VolumeStepSize{ volumeStepSize } {}


void Jotar::ChangeMusicVolume::Execute()
{
	int totalVolume = SoundServiceLocator::GetSoundSystem().GetMusicVolume() + m_VolumeStepSize;
	SoundServiceLocator::GetSoundSystem().SetMusicVolume(totalVolume);
}
