#include "DeathCheckerComponent.h"
#include "GameManager.h"

Jotar::DeathCheckerComponent::DeathCheckerComponent(GameObject* owner)
	: Component( owner )
	, m_AmountOfDeathPlayers{ 0 }
{
}


void Jotar::DeathCheckerComponent::OnNotify(const OnDeathEvent& eventData)
{
	if (typeid(eventData) == typeid(OnDeathEvent))
	{
		++m_AmountOfDeathPlayers;

		if (m_AmountOfDeathPlayers >= GameManager::GetInstance().GetPlayers().size())
		{
			GameManager::GetInstance().LoadLevel(false);
		}

	}

}
