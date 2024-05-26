#pragma once
#include "Event.h"
#include "ColliderComponent.h"
#include "GameObject.h"

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
		AIDeathEvent(GameObject* attacker)
			:m_Attacker{ attacker }
		{}

		virtual ~AIDeathEvent() = default;
		GameObject* GetAttacker() const { return m_Attacker; }

	private:
		GameObject* m_Attacker;
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


	class AIKilledTarget : public AIEvents
	{
	public:
		AIKilledTarget() = default;
		virtual ~AIKilledTarget() = default;
	};







}

