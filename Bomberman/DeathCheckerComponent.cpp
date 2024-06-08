#include "DeathCheckerComponent.h"
#include "GameManager.h"
#include "PlayerHealthComponent.h"

Jotar::DeathCheckerComponent::DeathCheckerComponent(GameObject* owner)
	: Component( owner )
	, m_AmountOfDeathPlayers{ 0 }
{
}


void Jotar::DeathCheckerComponent::OnNotify(const Event& eventData)
{
	if (typeid(eventData) == typeid(OnPlayerDeathEvent))
	{
		++m_AmountOfDeathPlayers;

		auto players = GameManager::GetInstance().GetPlayers();

		if (m_AmountOfDeathPlayers >= players.size())
		{
			int fullDeathPlayers = 0;

			for (size_t i = 0; i < players.size(); i++)
			{
				if (players[i]->GetOwner()->GetComponent< PlayerHealthComponent>()->GetHealth() < 0)
				{
					++fullDeathPlayers;
				}
			}

			if (fullDeathPlayers == players.size())
				GameManager::GetInstance().LoadHighScoreMenu(true);
			else
				GameManager::GetInstance().LoadLevel(false);
		}
	}

}
