#include "PlayerDeathComponent.h"
#include "MovementComponent.h"
#include "WorldTimeManager.h"
#include "PlaceBombComponent.h"

Jotar::PlayerDeathComponent::PlayerDeathComponent(GameObject* owner, float deathPauseTime)
	:Component( owner )
	, m_DeathPauseTime{ deathPauseTime  }
	, m_DeathTimer{ 0 }
	, m_IsDeath{ false }
{
	m_pSubject = std::make_unique<Subject<OnDeathEvent>>();
}

void Jotar::PlayerDeathComponent::Update()
{
	if (m_IsDeath)
	{
		m_DeathTimer += WorldTimeManager::GetInstance().GetDeltaTime();

		if (m_DeathTimer >= m_DeathPauseTime)
		{
			GetOwner()->GetTransform()->SetPosition(-200, -200);

			OnDeathEvent deathEvent{};
			m_pSubject->NotifyObservers(deathEvent);

			m_IsDeath = false;
		}
	}
}

void Jotar::PlayerDeathComponent::Reset()
{
	GetOwner()->GetComponent<MovementComponent>()->SetIsDisabled(false);
	GetOwner()->GetComponent<PlaceBombComponent>()->SetIsDisabled(false);
	m_pSubject->RemoveAllObservers();
}

void Jotar::PlayerDeathComponent::OnNotify(const HealthEvent& eventData)
{
	if (typeid(eventData) == typeid(DamageHealthEvent))
	{
		// stop Input
		GetOwner()->GetComponent<MovementComponent>()->SetIsDisabled(true);
		GetOwner()->GetComponent<PlaceBombComponent>()->SetIsDisabled(true);
		m_IsDeath = true;
	}
}
