#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include <vector>

namespace Jotar
{
	class GameObject;
	class  TransformComponent;

	class Camera : public Component 
	{
	public:
		explicit Camera(GameObject* owner, glm::ivec4& cameraRect, glm::ivec4& levelBoundaries);

		void LateUpdate() override;
		void SetTargets(std::vector<TransformComponent*> targets);

		glm::vec2 GetOffset();
		

	private:
		glm::ivec4 CalculateBoundingBox();
		float CalculateZoomFactor(const glm::ivec4& boundingBox);
		void SetZoom(float zoomFactor);
		void HandleWindowResize();


		glm::vec2 m_Offset;

		glm::ivec4 m_LevelBoundaries;
		glm::ivec4 m_CameraRect;
		std::vector<TransformComponent*> m_Targets;
	};

}