#include "UIButtonComponent.h"
#include "GameObject.h"

Jotar::UIButtonComponent::UIButtonComponent(GameObject* owner, const std::function<void()>& buttonFunction)
	: Component(owner)
	, m_ButtonFunction{ buttonFunction }
	, m_ButtonRect{}
{
}

void Jotar::UIButtonComponent::Start()
{
	m_ButtonRect = GetOwner()->GetTransform()->GetShape();
}

void Jotar::UIButtonComponent::PressButton()
{
	if (m_ButtonFunction)
	{
		m_ButtonFunction();
	}
}

bool Jotar::UIButtonComponent::IsPointInRect(const glm::ivec2& mousePos)
{
	return (mousePos.x >= m_ButtonRect.x &&
		mousePos.x <= m_ButtonRect.x + m_ButtonRect.w &&
		mousePos.y >= m_ButtonRect.y &&
		mousePos.y <= m_ButtonRect.y + m_ButtonRect.z);
}
