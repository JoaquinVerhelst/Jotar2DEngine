#include "BalloomPlayerHealthComponent.h"
#include "WorldTimeManager.h"
#include "MovementComponent.h"


Jotar::BalloomPlayerHealthComponent::BalloomPlayerHealthComponent(GameObject* owner, int health, float deathPauseTime)
	:HealthComponent(owner, health)
	, m_DeathPauseTime{ deathPauseTime }
	, m_DeathTimer{ 0 }
	, m_IsDeath{ false }
	, m_MaxHealth{ health }
{
	m_pOnDeathSubject = std::make_unique<Subject<AIDeathEvent>>();
}

void Jotar::BalloomPlayerHealthComponent::Update()
{
	if (m_IsDeath)
	{
		m_DeathTimer += WorldTimeManager::GetInstance().GetDeltaTime();

		if (m_DeathTimer >= m_DeathPauseTime)
		{
			// Just a position outside the view, these numbers mean nothing
			GetOwner()->GetTransform()->SetPosition(-1000, -1000);

			m_DeathTimer = 0;
			m_IsDeath = false;
		}
	}
}

void Jotar::BalloomPlayerHealthComponent::Reset()
{
	m_DeathTimer = 0;
	m_CurrentHealth = m_MaxHealth;
	GetOwner()->GetComponent<MovementComponent>()->SetIsDisabled(false);
}

void Jotar::BalloomPlayerHealthComponent::TakeDamage(int damage, GameObject* attacker)
{
	HealthComponent::TakeDamage(damage, attacker);

	GetOwner()->GetComponent<MovementComponent>()->SetIsDisabled(true);

	AIDeathEvent deathEvent{ attacker };
	m_pOnDeathSubject->NotifyObservers(deathEvent);


	m_IsDeath = true;
}
