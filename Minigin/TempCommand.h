#pragma once


#include "Command.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
//#include "steam_api.h"

namespace Jotar
{
	class TakeDamageCommand : public Command
	{
	public:
		TakeDamageCommand(HealthComponent* livesC)
			:m_pHealthComponent{ livesC }
		{}
		void Execute() override
		{
			m_pHealthComponent->TakeDamage();
		}

	private:
		HealthComponent* m_pHealthComponent;
	};

	class AddScoreCommand : public Command
	{
	public:
		AddScoreCommand(ScoreComponent* score)
			:m_pScoreComponent{ score }
		{}
		void Execute() override
		{
			// Temporary HardCoded Value
			m_pScoreComponent->AddScore(100);
		}

	private:
		ScoreComponent* m_pScoreComponent;
	};
}