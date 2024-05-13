#pragma once
#include "Event.h"
#include "ColliderComponent.h"


namespace Jotar
{
	class AIEvents : public Event
	{
	public:
		AIEvents() = default;
		virtual ~AIEvents() = default;
	};



	class AIDeathEvent : public AIEvents
	{
	public:
		AIDeathEvent() = default;
		virtual ~AIDeathEvent() = default;
	};


	class AIPlayerSeenEvent : public AIEvents
	{
	public:
		AIPlayerSeenEvent(ColliderComponent* targetCollider)
			: m_pTargetCollider{ targetCollider } {}
		virtual ~AIPlayerSeenEvent() = default;

		ColliderComponent* GetTargetCollider() const { return m_pTargetCollider; }

	private:

		ColliderComponent* m_pTargetCollider;
	};









}

