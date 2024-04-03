#include "EventPlayerDamage.h"

Jotar::EventPlayerDamage::EventPlayerDamage(int currentHealth)
	: m_Health{ currentHealth }
{
}

int Jotar::EventPlayerDamage::GetHealth() const
{
	return m_Health;
}
