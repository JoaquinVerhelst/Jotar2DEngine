#include "MenuComponent.h"
#include "GameObject.h"
#include "Scene.h"

#include "UIButtonComponent.h"
#include "TextComponent.h"
#include "Texture2D.h"


Jotar::MenuComponent::MenuComponent(GameObject* owner)
	: Component(owner)
	, m_ButtonChildren{}
{
}

void Jotar::MenuComponent::Update()
{

}

void Jotar::MenuComponent::AddButton(Scene& scene, std::string buttonName, const std::function<void()>& buttonFunction, glm::ivec4 rectBounds, const std::shared_ptr<Font>& font)
{
	auto buttonObj = scene.CreateGameObject(buttonName + "Button", false);
	buttonObj->AddComponent<TextComponent>(buttonName, font);
	auto buttonComp = buttonObj->AddComponent<UIButtonComponent>(buttonFunction);

	buttonObj->GetTransform()->SetPosition({ rectBounds.x, rectBounds.y });
	buttonObj->GetTransform()->SetSize({ rectBounds.w, rectBounds.z });


	m_ButtonChildren.emplace_back(buttonComp);
}

void Jotar::MenuComponent::ButtonSelectUp()
{
}

void Jotar::MenuComponent::ButtonSelectDown()
{
}
