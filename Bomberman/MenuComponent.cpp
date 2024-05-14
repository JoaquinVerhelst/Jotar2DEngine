#include "MenuComponent.h"
#include "GameObject.h"

#include "UIButtonComponent.h"
#include "TextComponent.h"
#include "Texture2D.h"
#include "InputManager.h"


#include <iostream>

Jotar::MenuComponent::MenuComponent(GameObject* owner)
	: Component(owner)
	, m_pButtonChildren{}
	, m_pMarkerObject{}
	, m_CurrentButtonIndex{ 0 }
{
}



void Jotar::MenuComponent::Start()
{


}

void Jotar::MenuComponent::Update() 
{
	if (InputManager::GetInstance().IsMouseButtonUp())
	{
		auto mousePos = InputManager::GetInstance().GetMouseLocation();

		for (size_t i = 0; i < m_pButtonChildren.size(); i++)
		{
			if (m_pButtonChildren[i]->IsPointInRect(mousePos)) {
				
				std::cout << " Button " << i << '\n';
				m_pButtonChildren[i]->PressButton();
			}
		}
	}

}

void Jotar::MenuComponent::AddButton(std::string buttonName, const std::function<void()>& buttonFunction, glm::ivec4  , const std::shared_ptr<Font>& font)
{
	auto buttonObj = GetOwner()->CreateChildGameObject(buttonName + "Button", false);
	buttonObj->AddComponent<TextComponent>(buttonName, font);


	auto buttonComp = buttonObj->AddComponent<UIButtonComponent>(buttonFunction);

	//buttonObj->GetTransform()->SetSize({ rectBounds.w, rectBounds.z });


	m_pButtonChildren.emplace_back(buttonComp);


	UpdateButtonPositions();
}

void Jotar::MenuComponent::SetMarkerChildObj(GameObject* pMarkerObj)
{
	m_pMarkerObject = pMarkerObj;
}

void Jotar::MenuComponent::ButtonSelectUp()
{
	++m_CurrentButtonIndex;
	SelectButton();
}

void Jotar::MenuComponent::ButtonSelectDown()
{
	--m_CurrentButtonIndex;
	SelectButton();
}

void Jotar::MenuComponent::SelectButton()
{
	if (m_CurrentButtonIndex >= m_pButtonChildren.size())
		m_CurrentButtonIndex = 0;
	else if (m_CurrentButtonIndex < 0)
		m_CurrentButtonIndex = static_cast<int>(m_pButtonChildren.size()) - 1;

	if (m_pButtonChildren.empty()) return;

	auto markerTransform = m_pMarkerObject->GetTransform();
	auto buttonObjPos = m_pButtonChildren[m_CurrentButtonIndex]->GetOwner()->GetTransform()->GetLocalPosition();

	markerTransform->SetPosition(buttonObjPos.x, buttonObjPos.y - markerTransform->GetSize().y);
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