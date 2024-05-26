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

		void Start() override;
		void Reset() override;
		void FixedUpdate() override;
		void OnDestroy() override;

		void Render() const override;



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

		void SetSize(glm::vec2 size);

		void OnTriggerBegin(TriggerBeginEvent& collisionEvent);
		void OnTriggerCollision(TriggerEvent& collisionEvent);
		void OnTriggerEnd(TriggerEndEvent& collisionEvent);

		void AddIgnoreCollisionTag(std::string tag);

		void OnColliderCollision(CollideEvent& collisionEvent);

		void UpdatePosition();

	private:

		bool IsColliderAlreadyHit(ColliderComponent* otherCollider);

		std::vector<std::string> m_IgnoreCollisionTags;

		glm::vec4 m_CollisionRect;
		std::string m_Tag;
		TransformComponent* m_pTransform;
		bool m_IsStatic;
		bool m_IsTrigger;
		bool m_HasCustomSize;
		std::unique_ptr<Subject<CollisionEvent>> m_pSubject;
		int m_SceneID;

		std::vector<ColliderComponent*> m_pCollidingColliders;
		std::vector<ColliderComponent*> m_pCollidingCollidersThisFrame;
	};
}