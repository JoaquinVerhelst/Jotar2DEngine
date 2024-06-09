#include "HUDComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "TextureComponent.h"



namespace Jotar
{

	const std::unordered_map<Jotar::HUDPosition, glm::vec2> Jotar::HUDComponent::s_PositionOffsets = {
      { HUDPosition::Center, glm::vec2(0.0f, 0.0f) },
        { HUDPosition::CenterUp, glm::vec2(0.0f, -0.5f) },
        { HUDPosition::CenterDown, glm::vec2(0.0f, 0.5f) },
        { HUDPosition::CenterRight, glm::vec2(0.5f, 0.0f) },
        { HUDPosition::RightUp, glm::vec2(0.5f, -0.5f) },
        { HUDPosition::RightDown, glm::vec2(0.5f, 0.5f) },
        { HUDPosition::CenterLeft, glm::vec2(-0.5f, 0.0f) },
        { HUDPosition::LeftUp, glm::vec2(-0.5f, -0.5f) },
        { HUDPosition::LeftDown, glm::vec2(-0.5f, 0.5f) },
	};
}

Jotar::HUDComponent::HUDComponent(GameObject* owner, HUDPosition position, glm::vec2 offset)
    : Component(owner)
    , m_Position{ position }
    , m_Offset{ offset }
{
	CalculateScreenPos();
}

void Jotar::HUDComponent::Start()
{
	Renderer::GetInstance().AddObserver(this);
}

void Jotar::HUDComponent::OnNotify(const WindowResizeEvent& )
{
    CalculateScreenPos();
}

void Jotar::HUDComponent::OnDestroy()
{
	Renderer::GetInstance().RemoveObserver(this);
}

void Jotar::HUDComponent::SetPosition(HUDPosition position, glm::vec2 offset)
{
    m_Position = position;
    m_Offset = offset;
    CalculateScreenPos();
}

void Jotar::HUDComponent::CalculateScreenPos()
{
    auto* parent = GetOwner()->GetParent();
    glm::ivec2 size = GetOwner()->GetTransform()->GetSize();
    glm::vec2 position;

    if (parent == nullptr)
    {
        auto& renderer = Renderer::GetInstance();
        int windowWidth, windowHeight;
        SDL_GetWindowSize(renderer.GetGlSDLManager()->GetWindow(), &windowWidth, &windowHeight);
        glm::vec2 screenSize = glm::vec2(windowWidth, windowHeight);
        glm::vec2 screenCenter = screenSize * 0.5f;
        position = CalculatePosition(m_Position, size, screenSize) + screenCenter + m_Offset;
    }
    else
    {
        glm::ivec2 parentSize = parent->GetTransform()->GetSize();
        glm::vec2 parentPos = parent->GetTransform()->GetWorldPosition();
   
        position = CalculatePosition(m_Position, size, glm::vec2(parentSize))+ m_Offset;
    }

    GetOwner()->GetTransform()->SetPosition(position.x, position.y);
}


glm::vec2 Jotar::HUDComponent::CalculatePosition(HUDPosition position, glm::ivec2 size, glm::vec2 referenceSize)
{
    auto it = s_PositionOffsets.find(position);
    if (it != s_PositionOffsets.end())
    {
        glm::vec2 normalizedPos = it->second;
        glm::vec2 absolutePos = glm::vec2(normalizedPos.x * referenceSize.x, normalizedPos.y * referenceSize.y);

        absolutePos.x -= normalizedPos.x * size.x;
        absolutePos.y -= normalizedPos.y * size.y;

        return absolutePos;
    }
    return glm::vec2(0.0f, 0.0f);
}