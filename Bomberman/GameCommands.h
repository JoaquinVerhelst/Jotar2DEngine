#pragma once
#include "Command.h"


namespace Jotar
{

	class GameObject;

	class SkipLevelCommand final : public Command
	{
	public:
		SkipLevelCommand() = default;
		virtual void Execute() override;
	};

	class GoToMainMenuCommand final : public Command
	{
	public:
		GoToMainMenuCommand() = default;
		virtual void Execute() override;
	};


	class MuteSoundCommand final : public Command
	{
	public:
		MuteSoundCommand() = default;
		virtual void Execute() override;
	};


	class ChangeSoundEffectVolume final : public Command
	{
	public:
		ChangeSoundEffectVolume(int volumeStepSize);
		virtual void Execute() override;

	private:
		int m_VolumeStepSize;
	};


	class ChangeMusicVolume final : public Command
	{
	public:
		ChangeMusicVolume(int volumeStepSize);
		virtual void Execute() override;

	private:
		int m_VolumeStepSize;
	};
}