#include "MovementComponent.h"
#include "WorldTimeManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "WorldGrid.h"

Jotar::MovementComponent::MovementComponent(GameObject* owner, float movementSpeed )
	: Component(owner)
	, m_MovementSpeed{movementSpeed}
{
	m_pTransformComponent = GetOwner()->GetTransform();
    
}

void Jotar::MovementComponent::Move(const glm::ivec2& dir)
{
	glm::vec2 ourPos{ m_pTransformComponent->GetLocalPosition() };

	auto newPos = Walk(dir, ourPos);
	//newPos += static_cast<glm::vec2>(dir) * m_MovementSpeed * WorldTimeManager::GetInstance().GetDeltaTime();

	m_pTransformComponent->SetPosition(newPos);
}

Jotar::TransformComponent* Jotar::MovementComponent::GetTransform()
{
	return m_pTransformComponent;
}



glm::vec2 Jotar::MovementComponent::Walk(glm::ivec2 direction, const glm::vec2& currentPosition)
{
	glm::vec2 newPos;

    if (direction.x != 0)
    {
		newPos = CalculateWalk(direction.x, currentPosition.y, currentPosition.x);
		return { newPos.y, newPos.x };
    }
    else if (direction.y != 0)
    {
		newPos = CalculateWalk(direction.y, currentPosition.x, currentPosition.y);
		return newPos;
    }

	return { 0,0 };
}
//
glm::vec2 Jotar::MovementComponent::CalculateWalk(int direction, float x, float y)
{
	float deltaTime = WorldTimeManager::GetInstance().GetDeltaTime();

	double offset = std::round((static_cast<int>(x) + 32) % static_cast<int>(64));

    glm::vec2 newPos = { x, y };

	if (offset == 0)
	{
        newPos.y += (direction == 1 ? m_MovementSpeed * deltaTime : -m_MovementSpeed * deltaTime);
        newPos.x = static_cast<float>(std::round(static_cast<int>(newPos.x)));
	}
	else
	{
        newPos.x += (offset >= 64 / 2.f ? m_MovementSpeed * deltaTime : -m_MovementSpeed * deltaTime);
        newPos.y = static_cast<float>(std::round(static_cast<int>(newPos.y)));
	}

	return newPos;

}