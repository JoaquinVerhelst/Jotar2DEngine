#pragma once
#include "Event.h"




namespace Jotar
{
	class ColliderComponent;


	class CollisionEvent : public Event
	{
	public:
		CollisionEvent(ColliderComponent* collider, ColliderComponent* otherCollider)
			: m_Collider{ collider }
			, m_OtherCollider{ otherCollider }
		{}
		
		virtual ~CollisionEvent() = default;

		ColliderComponent* GetCollider() { return m_Collider; }
		ColliderComponent* GetOtherCollider() { return m_OtherCollider; }

	protected:

		ColliderComponent* m_Collider;
		ColliderComponent* m_OtherCollider;

	};



	class TriggerEvent final : public CollisionEvent
	{
	public:
		TriggerEvent(ColliderComponent* collider, ColliderComponent* otherCollider)
			:CollisionEvent(collider, otherCollider) {}

		~TriggerEvent() = default;
	};


	class CollideEvent final : public CollisionEvent
	{
	public:
		CollideEvent(ColliderComponent* collider, ColliderComponent* otherCollider)
			:CollisionEvent(collider, otherCollider) {}

		~CollideEvent() = default;
	};

}
