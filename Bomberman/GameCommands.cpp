#include "GameCommands.h"

#include "SoundServiceLocator.h"
#include "SoundSystem.h"
#include "GameManager.h"


void Jotar::SkipLevelCommand::Execute()
{
	GameManager::GetInstance().LoadLevel();
}

void Jotar::MuteSoundCommand::Execute()
{
	SoundServiceLocator::GetSoundSystem().MuteSound();
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

void Jotar::GoToMainMenuCommand::Execute()
{
	GameManager::GetInstance().LoadMainMenu();
}
