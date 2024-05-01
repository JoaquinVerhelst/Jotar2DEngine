#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GameObject.h"
#include "GLSDLManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include <algorithm>


#include <iostream>

Jotar::Camera::Camera(GameObject* owner, glm::ivec4& cameraRect, glm::ivec4& levelBoundaries)
	: Component(owner)
	, m_CameraRect{ cameraRect }
	, m_LevelBoundaries{ levelBoundaries }
	, m_Target{nullptr}
    , m_Offset{0,0}
{

}


void Jotar::Camera::LateUpdate()
{
    if (m_Target == nullptr) return;

    glm::ivec4 targetShape = static_cast<glm::ivec4>(m_Target->GetShape());

    // Calculate the position of the player's center
    glm::ivec2 playerCenter = glm::ivec2(targetShape.x + targetShape.w / 2, targetShape.y + targetShape.z / 2);

    // Calculate the position of the camera's top-left corner
    glm::ivec2 cameraPosition = glm::ivec2(m_CameraRect.w / 2, m_CameraRect.z / 2) - playerCenter;

    // Clamp camera position to stay within the level boundaries
    //Keep the camera in bounds
    if (cameraPosition.x > 0)
    {
        cameraPosition.x = 0;
    }
    if (cameraPosition.y > 0)
    {
        cameraPosition.y = 0;
    }
    if (cameraPosition.x > m_LevelBoundaries.w - m_CameraRect.w)
    {
        cameraPosition.x = m_LevelBoundaries.w - m_CameraRect.w;
    }
    if (cameraPosition.y > m_LevelBoundaries.z - m_CameraRect.z)
    {
        cameraPosition.y = m_LevelBoundaries.z - m_CameraRect.z;
    }

    // Set the viewport of the renderer
    SDL_Rect camPos = { cameraPosition.x, cameraPosition.y, m_CameraRect.w, m_CameraRect.z };
    //Renderer::GetInstance().SetViewportPos(camPos);

    m_Offset = { cameraPosition.x, cameraPosition.y };





    //FOR SCALE -> ZOOMING IN AND OUT -> SDL_RenderSetScale
}

glm::ivec2 Jotar::Camera::Track(const glm::ivec4& target) const
{
    return glm::ivec2((target.x + target.w) / 2, (target.y + target.z) / 2);
}

void Jotar::Camera::SetTarget(TransformComponent* target)
{
    m_Target = target;
}

void Jotar::Camera::Clamp(glm::ivec2& targetPos) const
{
    targetPos.x = std::max(m_CameraRect.x, std::min(targetPos.x, m_LevelBoundaries.x + m_LevelBoundaries.z - m_CameraRect.w));
    targetPos.y = std::max(m_CameraRect.y, std::min(targetPos.y, m_LevelBoundaries.y + m_LevelBoundaries.w - m_CameraRect.z));
}

glm::vec2 Jotar::Camera::GetOffset()
{
    return m_Offset;
}