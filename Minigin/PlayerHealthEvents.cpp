#include "PlayerHealthEvents.h"

Jotar::EventPlayerDamage::EventPlayerDamage(int currentHealth)
	: m_Health{ currentHealth }
{
}

int Jotar::EventPlayerDamage::GetHealth() const
{
	return m_Health;
}

Jotar::EventUpdatePlayerHealth::EventUpdatePlayerHealth(int currentHealth)
	: m_Health{ currentHealth }
{}

int Jotar::EventUpdatePlayerHealth::GetHealth() const
{
	return m_Health;
}
