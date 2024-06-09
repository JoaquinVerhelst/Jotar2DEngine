#include "RotatorComponent.h"

#include "WorldTimeManager.h"


Jotar::RotatorComponent::RotatorComponent(GameObject* owner, const glm::vec2& center, float radius, float speed)
    : Jotar::Component(owner)
    , m_Center(center)
    , m_Radius(radius)
    , m_Speed(speed)
    , m_Angle(0.0f)
{
}


void Jotar::RotatorComponent::Update()
{
    RotatorLogic();
}

void Jotar::RotatorComponent::RotatorLogic()
{
    m_Angle += m_Speed * WorldTimeManager::GetInstance().GetDeltaTime();

    if (m_Angle >= 360.f)
    {
        m_Angle -= 360.f;
    }

    glm::vec2 offset = glm::vec2(std::cos(m_Angle), std::sin(m_Angle)) * m_Radius;
    GetOwner()->GetTransform()->SetPosition(m_Center.x + offset.x, m_Center.y + offset.y);


}
