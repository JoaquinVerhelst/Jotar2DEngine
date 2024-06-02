#include "CameraComponent.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GameObject.h"
#include "SDLManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include <algorithm>

#include "InputManager.h"
#include <iostream>

#include "SceneManager.h"
#include "Scene.h"


#include "SDL.h"

Jotar::CameraComponent::CameraComponent(GameObject* owner, glm::ivec4& cameraRect, glm::ivec4& levelBoundaries)
	: Component(owner)
	, m_CameraRect{ cameraRect }
	, m_LevelBoundaries{ levelBoundaries }
	, m_Target{nullptr}
    , m_Offset{0,0}
{

}


void Jotar::CameraComponent::Start()
{
    //SceneManager::GetInstance().GetCurrentScene().AddCamera(this);
}


void Jotar::CameraComponent::LateUpdate()
{
    HandleWindowResize();

    if (m_Target == nullptr)
        return;


    glm::ivec4 targetShape = static_cast<glm::ivec4>(m_Target->GetShape());

    // Calculate the position of the player's center
    glm::ivec2 playerCenter = glm::ivec2(targetShape.x + targetShape.w / 2, targetShape.y + targetShape.z / 2);

    // Calculate the position of the camera's top-left corner
    glm::ivec2 cameraPosition = glm::ivec2(m_CameraRect.w / 2, m_CameraRect.z / 2) - playerCenter;

    // Clamp camera position to stay within the level boundaries
    //Keep the camera in bounds
    if (cameraPosition.x > m_LevelBoundaries.x)
    {
        cameraPosition.x = m_LevelBoundaries.x;
    }
    if (cameraPosition.y > m_LevelBoundaries.y)
    {
        cameraPosition.y = m_LevelBoundaries.y;
    }
    if (cameraPosition.x > m_LevelBoundaries.w - m_CameraRect.w)
    {
        cameraPosition.x = m_LevelBoundaries.w - m_CameraRect.w;
    }
    if (cameraPosition.y > m_LevelBoundaries.z - m_CameraRect.z)
    {
        cameraPosition.y = m_LevelBoundaries.z - m_CameraRect.z;
    }



    m_Offset = { static_cast<float>(cameraPosition.x), static_cast<float>(cameraPosition.y) };
}

void Jotar::CameraComponent::SetTarget(TransformComponent* target)
{
    m_Target = target;
}

void Jotar::CameraComponent::SetLevelBounds(glm::ivec4& levelBoundaries)
{
    m_LevelBoundaries = levelBoundaries;
}

SDL_Rect Jotar::CameraComponent::GetCamRect()
{
    SDL_Rect camRect;
    camRect.x = m_CameraRect.x;
    camRect.y = m_CameraRect.y;
    camRect.w = m_CameraRect.w;
    camRect.h = m_CameraRect.z; 

    return camRect;
}

glm::vec2 Jotar::CameraComponent::GetOffset()
{
    return m_Offset;
}

glm::ivec4 Jotar::CameraComponent::CalculateBoundingBox()
{
    return glm::ivec4();
}

//TODO, REFACTOR, NOT INT ANYMORE< ITS ALREADY FLOATS
float Jotar::CameraComponent::CalculateZoomFactor(const glm::ivec4& boundingBox)
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

void Jotar::CameraComponent::SetZoom(float zoomFactor)
{
    // Adjust the logical size based on the zoom factor
    int logicalWidth = static_cast<int>(m_CameraRect.w * zoomFactor);
    int logicalHeight = static_cast<int>(m_CameraRect.z * zoomFactor);

    // Set the logical size for rendering
    SDL_RenderSetLogicalSize(Renderer::GetInstance().GetSDLRenderer(), logicalWidth, logicalHeight);

    // Set the scale for rendering
    SDL_RenderSetScale(Renderer::GetInstance().GetSDLRenderer(), 1.0f / zoomFactor, 1.0f / zoomFactor);
}

void Jotar::CameraComponent::HandleWindowResize()
{

}



