#pragma once
#include "Event.h"

namespace Jotar
{
	class HealthEvent : public Event
	{
	public:
		HealthEvent(int currentHealth)
			: m_Health{ currentHealth } {}
		virtual ~HealthEvent() = default;

		int GetHealth() const { return m_Health; }

	protected:
		int m_Health;
	};



	class DamageHealthEvent final : public HealthEvent
	{
	public:
		DamageHealthEvent(int currentHealth)
			:HealthEvent(currentHealth) {}
		virtual ~DamageHealthEvent() = default;
	};


	class UpdateHealthEvent final : public HealthEvent
	{
	public:
		UpdateHealthEvent(int currentHealth)
			:HealthEvent(currentHealth) {}
		virtual ~UpdateHealthEvent() = default;
	};

}