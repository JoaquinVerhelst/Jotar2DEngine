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
	, m_CollisionRect{}
	, m_pTransform{}
	, m_SceneID{0}
{
	m_pSubject = std::make_unique<Subject<CollisionEvent>>();
}

void Jotar::ColliderComponent::Start()
{
	m_SceneID = SceneManager::GetInstance().GetCurrentSceneID();
	SceneManager::GetInstance().GetCurrentScene().GetCollisionManager().AddCollider(this);

	m_pTransform = GetOwner()->GetTransform();

	auto& pos = m_pTransform->GetWorldPosition();
	auto& size = m_pTransform->GetSize();

	// x, y , z , w
	m_CollisionRect = { pos.x, pos.y, size.y, size.x };
}


void Jotar::ColliderComponent::OnDestroy()
{
	SceneManager::GetInstance().GetCurrentScene().GetCollisionManager().RemoveCollider(this);
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

void Jotar::ColliderComponent::SetIsTrigger(bool isTrigger)
{
	m_IsTrigger = isTrigger;
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


void Jotar::ColliderComponent::OnTriggerBegin(TriggerBeginEvent& beginOverlap)
{
	m_pSubject->NotifyObservers(beginOverlap);
}

void Jotar::ColliderComponent::OnTriggerCollision(TriggerEvent& triggerEvent)
{
	auto owner = GetOwner();
	owner->GetName();

	if (!IsColliderAlreadyHit(triggerEvent.GetOtherCollider()))
	{
		TriggerBeginEvent beginOverlap = TriggerBeginEvent{ triggerEvent.GetCollider(), triggerEvent.GetOtherCollider() };
		m_pCollidingColliders.emplace_back(triggerEvent.GetOtherCollider());
		OnTriggerBegin(beginOverlap);
	}

	m_pCollidingCollidersThisFrame.emplace_back(triggerEvent.GetOtherCollider());
}

void Jotar::ColliderComponent::OnTriggerEnd(TriggerEndEvent& endOverlap)
{
	m_pSubject->NotifyObservers(endOverlap);
}

void Jotar::ColliderComponent::AddIgnoreCollisionTag(std::string tag)
{
	m_IgnoreCollisionTags.emplace_back(tag);
}



void Jotar::ColliderComponent::OnColliderCollision(CollideEvent& collideEvent)
{
	m_pSubject->NotifyObservers(collideEvent);

	for (size_t i = 0; i < m_IgnoreCollisionTags.size(); i++)
	{
		if (collideEvent.GetOtherCollider()->CompareTag(m_IgnoreCollisionTags[i]))
			return;
	}

	const auto& otherCollisionRect = collideEvent.GetOtherCollider()->GetCollisionRect();
	glm::vec2 offset{};

	float horizontalOverlap = 0.0f;
	if (m_CollisionRect.x < otherCollisionRect.x)
		horizontalOverlap = otherCollisionRect.x - (m_CollisionRect.x + m_CollisionRect.w);
	else
		horizontalOverlap = (otherCollisionRect.x + otherCollisionRect.w) - m_CollisionRect.x;

	float verticalOverlap = 0.0f;
	if (m_CollisionRect.y < otherCollisionRect.y)
		verticalOverlap = otherCollisionRect.y - (m_CollisionRect.y + m_CollisionRect.z);
	else
		verticalOverlap = (otherCollisionRect.y + otherCollisionRect.z) - (m_CollisionRect.y);

	if (std::abs(horizontalOverlap) < std::abs(verticalOverlap)) {

		offset.x = horizontalOverlap;
	}
	else {

		offset.y = verticalOverlap;
	}

	GetOwner()->GetTransform()->Translate(offset);
}

void Jotar::ColliderComponent::UpdatePosition()
{
	auto& pos = GetOwner()->GetTransform()->GetWorldPosition();
	m_CollisionRect.x = pos.x;
	m_CollisionRect.y = pos.y;
}

void Jotar::ColliderComponent::SetTag(std::string tag)
{
	m_Tag = tag;
}

void Jotar::ColliderComponent::RemoveThisColliderFromManager()
{
	SceneManager::GetInstance().GetCurrentScene().GetCollisionManager().RemoveCollider(this);
}


void Jotar::ColliderComponent::Reset()
{
	SceneManager::GetInstance().GetSceneByID(m_SceneID).GetCollisionManager().RemoveCollider(this);
}

void Jotar::ColliderComponent::FixedUpdate()
{
	UpdatePosition();


	for (size_t i = 0; i < m_pCollidingColliders.size(); ++i)
	{
		auto foundObj = std::find_if(m_pCollidingCollidersThisFrame.begin(), m_pCollidingCollidersThisFrame.end(), [&](auto* p) { return p == m_pCollidingColliders[i]; });

		if (foundObj == m_pCollidingCollidersThisFrame.end())
		{
			TriggerEndEvent endOverlap = { this, m_pCollidingColliders[i] };
			OnTriggerEnd(endOverlap);
			m_pCollidingColliders.erase(m_pCollidingColliders.begin() + i);
		}
	}

	m_pCollidingCollidersThisFrame.clear();
}

bool Jotar::ColliderComponent::IsColliderAlreadyHit(ColliderComponent* otherCollider)
{
	for (size_t i = 0; i < m_pCollidingColliders.size(); i++)
	{
		if (m_pCollidingColliders[i] == otherCollider)
		{
			return true;
		}
	}

	return false;
}