#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include <string>
#include <functional>
#include <memory>

namespace Jotar
{
	class GameObject;
	class Font;
	class UIButtonComponent;

	class MenuComponent : public Component
	{
	public:
		MenuComponent(GameObject* owner, const glm::ivec3& defaultColor = glm::ivec3{255,255,255}, const  glm::ivec3& selectedColor = glm::ivec3{ 255,0,0 });
		~MenuComponent() = default;

		void Start() override;
		void Update() override;

		void AddButton(const std::string buttonName, const std::function<void()>& buttonFunction, const std::shared_ptr<Font>& font);

		void ButtonSelectUp();
		void ButtonSelectDown();

		void PressButton();

		MenuComponent(const MenuComponent& other) = delete;
		MenuComponent(MenuComponent&& other) = delete;
		MenuComponent& operator=(const MenuComponent& other) = delete;
		MenuComponent& operator=(MenuComponent&& other) = delete;

	private:

		void CheckMouseInput();

		void UpdateButtonPositions();
		void SelectButton(int index);




		std::vector<UIButtonComponent*> m_pButtonChildren;

		int m_CurrentButtonIndex;

		glm::ivec3 m_DefaultColor;
		glm::ivec3 m_SelectedColor;
	};
}
