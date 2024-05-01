#pragma once
#include <glm/glm.hpp>
#include "Component.h"


namespace Jotar
{
	class GameObject;
	class  TransformComponent;

	class Camera : public Component 
	{
	public:
		explicit Camera(GameObject* owner, glm::ivec4& cameraRect, glm::ivec4& levelBoundaries);

		void LateUpdate() override;
		void SetTarget(TransformComponent* target);

		glm::vec2 GetOffset();
		

	private:

		glm::ivec2 Track(const glm::ivec4& target) const;
		void Clamp(glm::ivec2& targetPos) const;


		glm::vec2 m_Offset;

		glm::ivec4 m_LevelBoundaries;
		glm::ivec4 m_CameraRect;
		TransformComponent* m_Target;
	};

}