#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include <string>
#include <functional>


namespace Jotar
{
	class GameObject;
	class ColliderComponent;

	class UIButtonComponent : public Component
	{
	public:
		UIButtonComponent(GameObject* owner, const std::function<void()>& buttonFunction);
		~UIButtonComponent() = default;

		void Render() const override;
		void Start() override;

		UIButtonComponent(const UIButtonComponent& other) = delete;
		UIButtonComponent(UIButtonComponent&& other) = delete;
		UIButtonComponent& operator=(const UIButtonComponent& other) = delete;
		UIButtonComponent& operator=(UIButtonComponent&& other) = delete;


		void PressButton();
		bool IsPointInRect(const glm::ivec2& mousePos);

	private:
		glm::ivec4 m_ButtonRect;
		std::function<void()> m_ButtonFunction;

	};
}