#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace Jotar
{
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* owner, bool moveWithCamera = true);
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

		void SetSize(glm::ivec2 size);
		glm::ivec2& GetSize();
		glm::vec4 GetShape();
		void SetPositionDirty();
		bool IsDirty() const { return m_IsPositionDirty; }

		void UpdateWorldPosition();
	private:


		bool m_IsPositionDirty;
		bool m_IsMovingWithCamera;

		glm::vec2 m_LocalPosition{};
		glm::vec2 m_WorldPosition{};

		glm::vec2 m_WorldProjectionPosition{};

		glm::ivec2 m_Size{ 64, 64 };
		int m_Scale = 1;
	};
}
