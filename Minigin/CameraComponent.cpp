#include "CameraComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
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
   
}


void Jotar::CameraComponent::LateUpdate()
{

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



