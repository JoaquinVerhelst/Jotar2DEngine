#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Component.h"
#include "TransformComponent.h"
#include <memory>
#include "Subject.h"
#include "CollisionEvents.h"


namespace Jotar
{
	class GameObject;

	class ColliderComponent final: public Component
	{
	public:
		ColliderComponent(GameObject* owner, bool isStatic = true, bool isTrigger = false);
		~ColliderComponent() = default;

		virtual void Start() override;
		virtual void FixedUpdate() override;
		virtual void OnDestroy() override;

		void AddObserver(Observer<CollisionEvent>* pObserver);
		void RemoveObserver(Observer<CollisionEvent>* pObserver);
		bool CompareTag(const std::string& compareTag) const;
		void SetTag(std::string tag);

		void RemoveThisColliderFromManager();

		glm::vec4 GetCollisionRect() const;
		void SetIsTrigger(bool isTrigger);
		bool GetIsTrigger() const;
		bool GetIsStatic() const;
		TransformComponent* GetTransform();

		bool IsOverlapping(const glm::vec4& otherCollisionRect);


		void OnTriggerBegin(TriggerBeginEvent& collisionEvent);
		void OnTriggerCollision(TriggerEvent& collisionEvent);
		void OnTriggerEnd(TriggerEndEvent& collisionEvent);


		void OnColliderCollision(CollideEvent& collisionEvent);

		void UpdatePosition();

	private:

		bool IsColliderAlreadyHit(ColliderComponent* otherCollider);


		glm::vec4 m_CollisionRect;
		std::string m_Tag;
		TransformComponent* m_pTransform;
		bool m_IsStatic;
		bool m_IsTrigger;
		std::unique_ptr<Subject<CollisionEvent>> m_pSubject;


		std::vector<ColliderComponent*> m_pCollidingColliders;
		std::vector<ColliderComponent*> m_pCollidingCollidersThisFrame;
	};
}