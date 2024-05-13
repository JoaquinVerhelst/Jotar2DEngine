#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GameObject.h"
#include "SDLManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include <algorithm>

#include "InputManager.h"

#include <iostream>

Jotar::Camera::Camera(GameObject* owner, glm::ivec4& cameraRect, glm::ivec4& levelBoundaries)
	: Component(owner)
	, m_CameraRect{ cameraRect }
	, m_LevelBoundaries{ levelBoundaries }
	, m_Targets{nullptr}
    , m_Offset{0,0}
{

}


void Jotar::Camera::LateUpdate()
{
    HandleWindowResize();

    if (m_Targets.empty())
        return;

    //glm::ivec4 boundingBox = CalculateBoundingBox();

    //// Calculate the center of the bounding box
    //glm::vec2 boundingBoxCenter = glm::ivec2(boundingBox.x + boundingBox.w / 2, boundingBox.y + boundingBox.z / 2);

    //// Calculate the position of the camera's center based on the bounding box center
    //glm::vec2 cameraCenter = glm::ivec2(m_CameraRect.w / 2, m_CameraRect.z / 2);

    //// Calculate the offset to move the camera to keep the bounding box centered
    //glm::vec2 cameraOffset = boundingBoxCenter - cameraCenter;

    //// Clamp camera position to stay within the level boundaries
    //cameraOffset.x = glm::clamp(cameraOffset.x, 0.f, m_LevelBoundaries.w - m_CameraRect.w);
    //cameraOffset.y = glm::clamp(cameraOffset.y, 0.f, m_LevelBoundaries.z - m_CameraRect.z);

    //// Update the camera's offset
    //m_Offset = cameraOffset;

    //// Calculate the required zoom level to fit all targets on screen
    //float zoomFactor = CalculateZoomFactor(boundingBox);

    //// Update the zoom level
    //SetZoom(zoomFactor);


    glm::ivec4 targetShape = static_cast<glm::ivec4>(m_Targets[0]->GetShape());

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

    m_Offset = { static_cast<float>(cameraPosition.x), static_cast<float>(cameraPosition.y) };
}

void Jotar::Camera::SetTargets(std::vector<TransformComponent*> targets)
{
    m_Targets = targets;
}

glm::vec2 Jotar::Camera::GetOffset()
{
    return m_Offset;
}

glm::ivec4 Jotar::Camera::CalculateBoundingBox()
{
    return glm::ivec4();
}

//TODO, REFACTOR, NOT INT ANYMORE< ITS ALREADY FLOATS
float Jotar::Camera::CalculateZoomFactor(const glm::ivec4& boundingBox)
{
    // Calculate the aspect ratio of the bounding box
    float boundingBoxAspectRatio = static_cast<float>(boundingBox.z) / static_cast<float>(boundingBox.w);

    // Calculate the aspect ratio of the camera's view
    float cameraAspectRatio = static_cast<float>(m_CameraRect.w) / static_cast<float>(m_CameraRect.z);
    float zoomFactor = 1.0f;

    if (boundingBoxAspectRatio > cameraAspectRatio)
    {
        zoomFactor = static_cast<float>(m_CameraRect.w) / static_cast<float>(boundingBox.z);
    }
    else
    {
        zoomFactor = static_cast<float>(m_CameraRect.z) / static_cast<float>(boundingBox.w);
    }

    zoomFactor = glm::clamp(zoomFactor, 0.3f, 1.5f);

    return zoomFactor;
}

void Jotar::Camera::SetZoom(float zoomFactor)
{
    // Adjust the logical size based on the zoom factor
    int logicalWidth = static_cast<int>(m_CameraRect.w * zoomFactor);
    int logicalHeight = static_cast<int>(m_CameraRect.z * zoomFactor);

    // Set the logical size for rendering
    SDL_RenderSetLogicalSize(Renderer::GetInstance().GetSDLRenderer(), logicalWidth, logicalHeight);

    // Set the scale for rendering
    SDL_RenderSetScale(Renderer::GetInstance().GetSDLRenderer(), 1.0f / zoomFactor, 1.0f / zoomFactor);
}

void Jotar::Camera::HandleWindowResize()
{

}



