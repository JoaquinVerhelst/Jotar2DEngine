#pragma once
#include <glm/glm.hpp>

namespace JV
{
	class Camera
	{
	public:
		Camera(float left, float right, float bottom, float top);

		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		// DONT FORGET, THIS NEEDS TO BE CALLED after CHANGING POS AND ROT OR WHATEVER, not implemented now cuz this is going to be refactored probs
		void CalculateViewMatrix();


		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

	};

}