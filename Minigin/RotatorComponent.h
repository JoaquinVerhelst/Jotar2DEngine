#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include "GameObject.h"

namespace Jotar
{

	class GameObject;

	class RotatorComponent final : public Component
	{
	public:
		void Update() override;

		RotatorComponent(GameObject* owner, const glm::vec2& center, float radius, float speed);
		~RotatorComponent() = default;

		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		void RotatorLogic();

	private:


		glm::vec2 m_Center;
		float m_Radius;
		float m_Speed;
		float m_Angle;
		const float m_PiDivTwo{ 1.57079632679f };
	};
}