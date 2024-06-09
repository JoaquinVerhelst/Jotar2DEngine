#include "MenuComponent.h"
#include "GameObject.h"

#include "UIButtonComponent.h"
#include "TextComponent.h"
#include "Texture2D.h"
#include "InputManager.h"
#include "HUDComponent.h"


#include <iostream>


Jotar::MenuComponent::MenuComponent(GameObject* owner, const  glm::ivec3& defaultColor, const  glm::ivec3& selectedColor)
	: Component(owner)
	, m_pButtonChildren{}
	, m_CurrentButtonIndex{ 0 }
	, m_DefaultColor{ defaultColor }
	, m_SelectedColor{ selectedColor }
{
}

void Jotar::MenuComponent::Start()
{


}

void Jotar::MenuComponent::Update() 
{
	CheckMouseInput();
}


void Jotar::MenuComponent::AddButton(const std::string buttonName, const std::function<void()>& buttonFunction  , const std::shared_ptr<Font>& font)
{
	auto buttonObj = GetOwner()->CreateChildGameObject(buttonName + "Button", false);
	buttonObj->AddComponent<TextComponent>(buttonName, font);
	buttonObj->AddComponent<HUDComponent>();
	auto buttonComp = buttonObj->AddComponent<UIButtonComponent>(buttonFunction);

	m_pButtonChildren.emplace_back(buttonComp);

	UpdateButtonPositions();
}


void Jotar::MenuComponent::ButtonSelectUp()
{
	m_pButtonChildren[m_CurrentButtonIndex]->ChangeColor(m_DefaultColor);
	--m_CurrentButtonIndex;
	SelectButton(m_CurrentButtonIndex);
}

void Jotar::MenuComponent::ButtonSelectDown()
{
	m_pButtonChildren[m_CurrentButtonIndex]->ChangeColor(m_DefaultColor);
	++m_CurrentButtonIndex;
	SelectButton(m_CurrentButtonIndex);
}

void Jotar::MenuComponent::PressButton()
{
	m_pButtonChildren[m_CurrentButtonIndex]->PressButton();
}

void Jotar::MenuComponent::SelectButton(int index)
{
	m_CurrentButtonIndex = index;

	if (m_CurrentButtonIndex >= static_cast<int>(m_pButtonChildren.size()))
		m_CurrentButtonIndex = 0;
	if (m_CurrentButtonIndex < 0)
		m_CurrentButtonIndex = static_cast<int>(m_pButtonChildren.size() - 1);

	if (m_pButtonChildren.empty()) return;

	m_pButtonChildren[m_CurrentButtonIndex]->ChangeColor(m_SelectedColor);
}

void Jotar::MenuComponent::UpdateButtonPositions()
{
	auto amountOfButtons = m_pButtonChildren.size();
	if (amountOfButtons == 0) return;

	float spacing = 40.0f;
	float totalHeight = (amountOfButtons - 1) * spacing;
	float startY = totalHeight / 2.0f;

	for (size_t i = 0; i < amountOfButtons; ++i)
	{
		auto transform = m_pButtonChildren[i]->GetOwner()->GetTransform();

		auto size = transform->GetSize();
		float posY = startY - i * -spacing;
		transform->SetPosition(static_cast<float>(-size.x) / 2.f, posY);
	}
}

void Jotar::MenuComponent::CheckMouseInput()
{
	auto mousePos = InputManager::GetInstance().GetMouseLocation();

	for (size_t i = 0; i < m_pButtonChildren.size(); i++)
	{
		if (m_pButtonChildren[i]->IsPointInRect(mousePos)) {

			if (m_CurrentButtonIndex != static_cast<int>(i))
			{
				m_pButtonChildren[m_CurrentButtonIndex]->ChangeColor(m_DefaultColor);
				SelectButton(static_cast<int>(i));
			}

			if (InputManager::GetInstance().IsMouseButtonUp())
				PressButton();
		}
	}
}
