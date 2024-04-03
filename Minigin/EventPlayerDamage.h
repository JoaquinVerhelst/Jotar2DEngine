#pragma once
#include "Event.h"

namespace Jotar
{
	class EventPlayerDamage final : public Event
	{
	public:
		EventPlayerDamage(int currentHealth);
		virtual ~EventPlayerDamage() = default;

		int GetHealth() const;

	private:
		int m_Health;
	};
}