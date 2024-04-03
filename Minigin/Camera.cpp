#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

JV::Camera::Camera(float left, float right, float bottom, float top)
	: m_ProjectionMatrix(glm::ortho(left,right, bottom, top, -1.f ,1.f))
	, m_ViewMatrix{}
	, m_ViewProjectionMatrix{}
{
}

void JV::Camera::CalculateViewMatrix()
{
	glm::vec3 pos = {};
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * 
		glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0, 0, 1));

	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
