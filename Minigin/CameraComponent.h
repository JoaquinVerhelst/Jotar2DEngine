#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include <vector>

namespace Jotar
{
	class GameObject;
	class  TransformComponent;

	class CameraComponent : public Component 
	{
	public:
		explicit CameraComponent(GameObject* owner, glm::ivec4& cameraRect, glm::ivec4& levelBoundaries);

		void Start() override;
		void LateUpdate() override;
		void SetTargets(std::vector<TransformComponent*> targets);

		void SetLevelBounds(glm::ivec4& levelBoundaries);

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