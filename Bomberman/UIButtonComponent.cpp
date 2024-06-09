#include "UIButtonComponent.h"
#include "GameObject.h"
#include "TextComponent.h"


#include "SDL.h"
#include "Renderer.h"
#include <cstdlib> // For rand() and srand()
#include <ctime> // For time()

Jotar::UIButtonComponent::UIButtonComponent(GameObject* owner, const std::function<void()>& buttonFunction)
	: Component(owner)
	, m_ButtonFunction{ buttonFunction }
	, m_ButtonRect{}
	, m_pTextComponent{}
{
}

void Jotar::UIButtonComponent::Start()
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
	m_ButtonRect = GetOwner()->GetTransform()->GetShape();
}


void Jotar::UIButtonComponent::PressButton()
{
	if (m_ButtonFunction)
	{
		m_ButtonFunction();
	}
}

void Jotar::UIButtonComponent::ChangeColor(const glm::ivec3& color)
{
	m_pTextComponent->SetColor(color);
}

bool Jotar::UIButtonComponent::IsPointInRect(const glm::ivec2& mousePos)
{
	m_ButtonRect = GetOwner()->GetTransform()->GetShape();

	return (mousePos.x >= m_ButtonRect.x &&
		mousePos.x <= m_ButtonRect.x + m_ButtonRect.w &&
		mousePos.y >= m_ButtonRect.y &&
		mousePos.y <= m_ButtonRect.y + m_ButtonRect.z);
}
