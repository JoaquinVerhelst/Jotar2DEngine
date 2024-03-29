#include "TransformComponent.h"
#include "GameObject.h"


Jotar::TransformComponent::TransformComponent(GameObject* owner)
	:Component(owner)
	, m_IsPositionDirty{false}
{
}


const glm::vec2& Jotar::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec2& Jotar::TransformComponent::GetWorldPosition()
{
	if (m_IsPositionDirty)
		UpdateWorldPosition();
	
	return m_WorldPosition;
}

void Jotar::TransformComponent::SetPosition(float x, float y)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;

	SetPositionDirty();
}

void Jotar::TransformComponent::SetPosition(const glm::vec2& position)
{
	m_LocalPosition = position;

	SetPositionDirty();
}


void Jotar::TransformComponent::SetPositionDirty()
{
	const auto pChildren = GetOwner()->GetChildren();

	for (pChildren; const auto & child : pChildren)
	{
		child->GetTransform()->SetPositionDirty();
	}

	m_IsPositionDirty = true;
}

void Jotar::TransformComponent::UpdateWorldPosition()
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
