#include "HealthComponent.h"

Jotar::HealthComponent::HealthComponent(GameObject* owner, int health)
	:Component(owner)
	, m_CurrentHealth(health)
{
	m_pSubject = std::make_unique<Subject<HealthEvent>>();
}

Jotar::HealthComponent::~HealthComponent()
{
	m_pSubject->RemoveAllObservers();
}

void Jotar::HealthComponent::Start()
{
	m_pSubject->NotifyObservers(UpdateHealthEvent(m_CurrentHealth));
}

int Jotar::HealthComponent::GetHealth() const
{
	return m_CurrentHealth;
}

void Jotar::HealthComponent::TakeDamage(int damage, GameObject* attacker)
{
	m_CurrentHealth -= damage;

	m_pSubject->NotifyObservers(DamageHealthEvent{m_CurrentHealth, attacker});


	// stop moving


	// reloadlevel


}


void Jotar::HealthComponent::AddHealth(int health)
{
	m_CurrentHealth += health;
}
