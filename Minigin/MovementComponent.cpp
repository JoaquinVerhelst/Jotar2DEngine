#include "MovementComponent.h"
#include "WorldTimeManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

Jotar::MovementComponent::MovementComponent(GameObject* owner, float movementSpeed)
	: Component(owner)
	, m_MovementSpeed{movementSpeed}
{
	m_pTransformComponent = GetOwner()->GetComponent<TransformComponent>();
}

void Jotar::MovementComponent::Move(const glm::vec2& dir)
{
	glm::vec2 newPos{ m_pTransformComponent->GetLocalPosition() };
	newPos += dir * m_MovementSpeed * WorldTimeManager::GetInstance().GetDeltaTime();

	m_pTransformComponent->SetPosition(newPos);
}
