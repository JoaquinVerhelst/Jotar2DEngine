#include "ExitComponent.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "GameManager.h"
#include <iostream>

#include "SoundServiceLocator.h"
#include "SoundSystem.h"


Jotar::ExitComponent::ExitComponent(GameObject* owner, std::string exitTextureFilePath)
	: Component(owner)
	, m_EnemiesRemaining{0}
	, m_ExitTextureFilePath{ exitTextureFilePath }
	, m_IsExitRevealed{ false }
	, m_ShouldLoadNextLevel{ false }
{
}

void Jotar::ExitComponent::SetAmountOfEnemies(int totalEnemies)
{
	m_EnemiesRemaining = totalEnemies;
}


void Jotar::ExitComponent::OnNotify(const AIDeathEvent& aiDeathEvent)
{
	if (typeid(aiDeathEvent) == typeid(AIDeathEvent))
	{
		--m_EnemiesRemaining;

		if (m_EnemiesRemaining == 0)
		{
			RevealExit();
		}
	}
}

void Jotar::ExitComponent::OnNotify(const CollisionEvent& triggerEvent)
{
	if (!m_IsExitRevealed) return;

	if (typeid(triggerEvent) == typeid(TriggerBeginEvent))
	{
		if (triggerEvent.GetOtherCollider() == nullptr) return;

		auto* otherCollider = triggerEvent.GetOtherCollider();

		if (otherCollider->CompareTag("Player"))
		{
			GameManager::GetInstance().LoadLevel(true);
		}
	}
}
 
void Jotar::ExitComponent::RevealExit()
{
	SoundServiceLocator::GetSoundSystem().Play("ExitOpens");

	// set the texture
	GetOwner()->GetComponent<TextureComponent>()->SetTexture(m_ExitTextureFilePath);

	// ->Set it to trigger
	GetOwner()->GetComponent<ColliderComponent>()->SetIsTrigger(true);
	m_IsExitRevealed = true;
}
