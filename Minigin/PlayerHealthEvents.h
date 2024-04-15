#pragma once
#include "Event.h"

namespace Jotar
{
	class PlayerHealthEvent : public Event
	{
	public:
		PlayerHealthEvent() = default;
		virtual ~PlayerHealthEvent() = default;

	};



	class EventPlayerDamage final : public PlayerHealthEvent
	{
	public:
		EventPlayerDamage(int currentHealth);
		virtual ~EventPlayerDamage() = default;

		int GetHealth() const;

	private:
		int m_Health;
	};


	class EventUpdatePlayerHealth final : public PlayerHealthEvent
	{
	public:
		EventUpdatePlayerHealth(int currentHealth);
		virtual ~EventUpdatePlayerHealth() = default;

		int GetHealth() const;

	private:
		int m_Health;
	};

}