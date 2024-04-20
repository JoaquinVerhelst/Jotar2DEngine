#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace Jotar
{
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* owner);
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		const glm::vec2& GetLocalPosition() const;
		const glm::vec2& GetWorldPosition();


		void SetPosition(float x, float y);
		void SetPosition(const glm::vec2& position);
		void Translate(const glm::vec2& position);

		void SetSize(glm::vec2& size);
		glm::vec2& GetSize();


		void SetPositionDirty();

		bool IsDirty() const { return m_IsPositionDirty; }

	private:
		void UpdateWorldPosition();

		bool m_IsPositionDirty;

		glm::vec2 m_LocalPosition{};
		glm::vec2 m_WorldPosition{};

		glm::vec2 m_Size{ 64, 64 };
		int m_Scale = 1;
	};
}
