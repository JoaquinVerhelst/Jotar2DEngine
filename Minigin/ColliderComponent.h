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

		glm::vec4 GetCollisionRect() const;
		bool GetIsTrigger() const;
		bool GetIsStatic() const;
		TransformComponent* GetTransform();

		bool IsOverlapping(const glm::vec4& otherCollisionRect);
		void OnTriggerCollision(TriggerEvent& collisionEvent);

	private:
		void UpdatePosition();

		glm::vec4 m_CollisionRect;
		std::string m_Tag;
		TransformComponent* m_pTransform;
		bool m_IsStatic;
		bool m_IsTrigger;
		std::unique_ptr<Subject<CollisionEvent>> m_pSubject;
	};
}