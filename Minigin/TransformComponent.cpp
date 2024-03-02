#include "TransformComponent.h"
#include "GameObject.h"


dae::TransformComponent::TransformComponent(GameObject* owner)
	:Component(owner)
	, m_IsPositionDirty{false}
{
}


const glm::vec2& dae::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec2& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsPositionDirty)
		UpdateWorldPosition();
	
	return m_WorldPosition;
}

void dae::TransformComponent::SetLocalPosition(float x, float y)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;

	SetPositionDirty();
}

void dae::TransformComponent::SetLocalPosition(const glm::vec2& position)
{
	m_LocalPosition = position;

	SetPositionDirty();
}


void dae::TransformComponent::SetPositionDirty()
{
	const auto pChildren = GetOwner()->GetChildren();

	for (pChildren; const auto & child : pChildren)
	{
		child->GetTransform()->SetPositionDirty();
	}

	m_IsPositionDirty = true;
}

void dae::TransformComponent::UpdateWorldPosition()
{
	if (m_IsPositionDirty)
	{
		const auto parent = GetOwner()->GetParent();

		if (parent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = parent->GetTransform()->GetWorldPosition() + m_LocalPosition;
	}

	m_IsPositionDirty = false;
}
