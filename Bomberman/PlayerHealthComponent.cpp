#include "PlayerHealthComponent.h"
#include "MovementComponent.h"
#include "ColliderComponent.h"
#include "WorldTimeManager.h"
#include "PlaceBombComponent.h"
#include "GameManager.h"

#include "SoundServiceLocator.h"
#include "SoundSystem.h"



Jotar::PlayerHealthComponent::PlayerHealthComponent(GameObject* owner, int health, float deathPauseTime)
	:HealthComponent( owner, health)
	, m_DeathPauseTime{ deathPauseTime  }
	, m_DeathTimer{ 0 }
	, m_IsDeath{ false }
{
	m_pOnDeathSubject = std::make_unique<Subject<Event>>();
}

void Jotar::PlayerHealthComponent::Update()
{
	if (m_IsDeath)
	{
		m_DeathTimer += WorldTimeManager::GetInstance().GetDeltaTime();

		if (m_DeathTimer >= m_DeathPauseTime)
		{
			GetOwner()->GetTransform()->SetPosition(-200, -200);
			OnPlayerDeathEvent deathEvent{ GetOwner() };
			m_pOnDeathSubject->NotifyObservers(deathEvent);

			m_DeathTimer = 0;
			m_IsDeath = false;
		}
	}
}

void Jotar::PlayerHealthComponent::Reset()
{
	m_DeathTimer = 0;

	if (m_CurrentHealth >= 0)
	{
		GetOwner()->GetComponent<MovementComponent>()->SetIsDisabled(false);
		GetOwner()->GetComponent<PlaceBombComponent>()->SetIsDisabled(false);
		GetOwner()->GetComponent<ColliderComponent>()->SetIsDisabled(false);
		m_pOnDeathSubject->RemoveAllObservers();
	}
}

void Jotar::PlayerHealthComponent::TakeDamage(int damage, GameObject* attacker)
{
	HealthComponent::TakeDamage(damage, attacker);


	SoundServiceLocator::GetSoundSystem().Play("PlayerDies");

	// stop Input
	GetOwner()->GetComponent<MovementComponent>()->SetIsDisabled(true);
	GetOwner()->GetComponent<PlaceBombComponent>()->SetIsDisabled(true);
	GetOwner()->GetComponent<ColliderComponent>()->SetIsDisabled(true);
	m_IsDeath = true;
}

