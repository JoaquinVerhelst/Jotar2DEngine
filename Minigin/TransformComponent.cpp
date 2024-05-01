#include "TransformComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Camera.h"


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
	
	m_WorldProjectionPosition = m_WorldPosition;

	//todo get rid of hardcocded index
	auto camObj = SceneManager::GetInstance().GetScene(0).GetCamera();
	if (camObj != nullptr)
		m_WorldProjectionPosition += camObj->GetOffset();



	if (GetOwner()->GetName() == ("Camera"))
		return m_WorldPosition;
	else
		return m_WorldProjectionPosition;
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

void Jotar::TransformComponent::Translate(const glm::vec2& position)
{
	m_LocalPosition += position;

	SetPositionDirty();
}

void Jotar::TransformComponent::SetSize(glm::vec2& size)
{
	m_Size = size;
}

glm::vec2& Jotar::TransformComponent::GetSize()
{
	return m_Size;
}

glm::vec4 Jotar::TransformComponent::GetShape()
{
	return glm::vec4(m_WorldPosition.x, m_WorldPosition.y, m_Size.y, m_Size.x);
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
