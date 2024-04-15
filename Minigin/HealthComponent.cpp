#include "HealthComponent.h"

Jotar::HealthComponent::HealthComponent(GameObject* owner, int health)
	:Component(owner)
	, m_CurrentHealth(health)
{
	m_pSubject = std::make_unique<Subject<PlayerHealthEvent>>();
}

Jotar::HealthComponent::~HealthComponent()
{
	m_pSubject->RemoveAllObservers();
}

void Jotar::HealthComponent::Start()
{
	m_pSubject->NotifyObservers(EventUpdatePlayerHealth(m_CurrentHealth));
}

int Jotar::HealthComponent::GetHealth() const
{
	return m_CurrentHealth;
}

void Jotar::HealthComponent::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;

	m_pSubject->NotifyObservers(EventPlayerDamage{m_CurrentHealth});
}


void Jotar::HealthComponent::AddHealth(int health)
{
	m_CurrentHealth += health;
}
