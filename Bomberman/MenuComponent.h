#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include <string>
#include <functional>
#include <memory>

namespace Jotar
{
	class GameObject;
	class Scene;
	class Font;
	class UIButtonComponent;

	class MenuComponent : public Component
	{
	public:
		MenuComponent(GameObject* owner);
		~MenuComponent() = default;


		void Update() override;


		void AddButton(Scene& scene, std::string buttonName, const std::function<void()>& buttonFunction, glm::ivec4 rectBonds, const std::shared_ptr<Font>& font);

		void ButtonSelectUp();
		void ButtonSelectDown();

		MenuComponent(const MenuComponent& other) = delete;
		MenuComponent(MenuComponent&& other) = delete;
		MenuComponent& operator=(const MenuComponent& other) = delete;
		MenuComponent& operator=(MenuComponent&& other) = delete;

	private:

		std::vector<UIButtonComponent*> m_ButtonChildren;

	};
}
