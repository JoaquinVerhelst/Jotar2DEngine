#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include <vector>


struct SDL_Rect;


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
		void SetTarget(TransformComponent* target);

		void SetLevelBounds(glm::ivec4& levelBoundaries);

		SDL_Rect GetCamRect();

		glm::vec2 GetOffset();
		

	private:
		glm::ivec4 CalculateBoundingBox();
		float CalculateZoomFactor(const glm::ivec4& boundingBox);
		void SetZoom(float zoomFactor);
		void HandleWindowResize();


		glm::vec2 m_Offset;

		glm::ivec4 m_LevelBoundaries;
		glm::ivec4 m_CameraRect;
		TransformComponent* m_Target;
	};

}