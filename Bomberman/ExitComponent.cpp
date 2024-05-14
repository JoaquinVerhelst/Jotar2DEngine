#include "ExitComponent.h"

#include "GameObject.h"

#include <iostream>

Jotar::ExitComponent::ExitComponent(GameObject* owner, int amountOfEnemies)
	: Component(owner)
	, m_EnemiesRemaining{amountOfEnemies}
{

}

void Jotar::ExitComponent::OnNotify(const AIDeathEvent& aiDeathEvent)
{
	if (typeid(aiDeathEvent) == typeid(AIDeathEvent))
	{
		--m_EnemiesRemaining;

		if (m_EnemiesRemaining == 0)
		{
			std::cout << "Won" << '\n';
		}
	}
}
