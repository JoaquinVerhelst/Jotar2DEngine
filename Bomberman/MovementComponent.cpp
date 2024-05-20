#include "MovementComponent.h"
#include "WorldTimeManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "WorldGrid.h"

Jotar::MovementComponent::MovementComponent(GameObject* owner, float movementSpeed, int cellsize)
	: Component(owner)
	, m_MovementSpeed{movementSpeed}
	, m_CellSize{ cellsize}
{
	m_pTransformComponent = GetOwner()->GetTransform();
    
}

void Jotar::MovementComponent::Move(const glm::ivec2& dir)
{
	glm::vec2 ourPos{ m_pTransformComponent->GetLocalPosition() };

	auto newPos = Walk(dir, ourPos);
	m_pTransformComponent->Translate(newPos);
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
		newPos = CalculateWalk(direction.x, currentPosition.y);
		return { newPos.y, newPos.x };
    }
    else if (direction.y != 0)
    {
		newPos = CalculateWalk(direction.y, currentPosition.x);
		return newPos;
    }

	return { 0,0 };
}
//
glm::vec2 Jotar::MovementComponent::CalculateWalk(int direction, float axis )
{
	float deltaTime = WorldTimeManager::GetInstance().GetDeltaTime();

	double offset = std::round((static_cast<int>(axis) + m_CellSize/2) % static_cast<int>(m_CellSize));

    glm::vec2 newPos{ };

	if (offset == 0)
	{
        newPos.y = (direction == 1 ? m_MovementSpeed * deltaTime : -m_MovementSpeed * deltaTime);
        newPos.x = 0;
	}
	else
	{
        newPos.x = (offset >= m_CellSize / 2.f ? m_MovementSpeed * deltaTime : -m_MovementSpeed * deltaTime);
        newPos.y = 0;
	}

	return newPos;

}