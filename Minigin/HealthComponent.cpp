#include "HealthComponent.h"

Jotar::HealthComponent::HealthComponent(GameObject* owner, int health)
	:Component(owner)
	, m_CurrentHealth(health)
{
}

Jotar::HealthComponent::~HealthComponent()
{
	RemoveAllObservers();
}

void Jotar::HealthComponent::Start()
{
	// wrong event, player Spawn Event OR 
	NotifyObservers(new EventPlayerDamage{ m_CurrentHealth }); // TODO; NEW weg, 
}

int Jotar::HealthComponent::GetHealth() const
{
	return m_CurrentHealth;
}

void Jotar::HealthComponent::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;


	NotifyObservers(new EventPlayerDamage{m_CurrentHealth});// tODO new weg
}


void Jotar::HealthComponent::AddHealth(int health)
{
	m_CurrentHealth += health;
}
