#include "ColliderComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "Scene.h"

#include <iostream>

Jotar::ColliderComponent::ColliderComponent(GameObject* owner, bool isStatic, bool isTrigger)
	: Component(owner)
	, m_IsStatic{ isStatic }
	, m_IsTrigger{ isTrigger }
{
	m_pSubject = std::make_unique<Subject<CollisionEvent>>();


	m_pTransform = GetOwner()->GetComponent<TransformComponent>();

	auto& pos = m_pTransform->GetWorldPosition();
	auto& size = m_pTransform->GetSize();

	// x, y , z , w
	m_CollisionRect = { pos.x, pos.y, size.y, size.x };

	SceneManager::GetInstance().GetScene(0).GetCollisionManager().AddCollider(this);
}

void Jotar::ColliderComponent::OnDestroy()
{
	//to do : Get Rid of hard coded scene index
	SceneManager::GetInstance().GetScene(0).GetCollisionManager().RemoveCollider(this);
	m_pSubject->RemoveAllObservers();
}

void Jotar::ColliderComponent::AddObserver(Observer<CollisionEvent>* pObserver)
{
	m_pSubject->AddObserver(pObserver);
}

void Jotar::ColliderComponent::RemoveObserver(Observer<CollisionEvent>* pObserver)
{
	m_pSubject->RemoveObserver(pObserver);
}

bool Jotar::ColliderComponent::CompareTag(const std::string& compareTag) const
{
	if (m_Tag == compareTag)
		return true;

	return false;
}

glm::vec4 Jotar::ColliderComponent::GetCollisionRect() const
{
	return m_CollisionRect;
}

bool Jotar::ColliderComponent::GetIsTrigger() const
{
	return m_IsTrigger;
}

bool Jotar::ColliderComponent::GetIsStatic() const
{
	return m_IsStatic;
}

Jotar::TransformComponent* Jotar::ColliderComponent::GetTransform()
{
	return m_pTransform;
}

bool Jotar::ColliderComponent::IsOverlapping(const glm::vec4& otherCollisionRect)
{
	if (otherCollisionRect.x + otherCollisionRect.w > m_CollisionRect.x &&
		otherCollisionRect.x < m_CollisionRect.x + m_CollisionRect.w &&
		otherCollisionRect.y + otherCollisionRect.z > m_CollisionRect.y && 
		otherCollisionRect.y < m_CollisionRect.y + m_CollisionRect.z)
	{
		return true;
	}

	return false;
}

void Jotar::ColliderComponent::OnTriggerCollision(TriggerEvent& collisionEvent)
{
	m_pSubject->NotifyObservers(collisionEvent);
}

void Jotar::ColliderComponent::UpdatePosition()
{
	auto& pos = m_pTransform->GetWorldPosition();
	m_CollisionRect.x = pos.x;
	m_CollisionRect.y = pos.y;
}

void Jotar::ColliderComponent::SetTag(std::string tag)
{
	m_Tag = tag;
}

void Jotar::ColliderComponent::Start()
{
	UpdatePosition();
}

void Jotar::ColliderComponent::FixedUpdate()
{
	if (!m_IsStatic)
	{
		UpdatePosition();
	}
}

