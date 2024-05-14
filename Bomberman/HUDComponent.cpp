#include "HUDComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "TextureComponent.h"



namespace Jotar
{

	const std::unordered_map<Jotar::HUDPosition, glm::vec2> Jotar::HUDComponent::s_PositionOffsets = {
		{ HUDPosition::Center, glm::vec2(0.5f, 0.5f) },
		{ HUDPosition::CenterUp, glm::vec2(0.5f, 0.0f) },
		{ HUDPosition::CenterDown, glm::vec2(0.5f, 1.0f) },
		{ HUDPosition::CenterRight, glm::vec2(1.0f, 0.5f) },
		{ HUDPosition::RightUp, glm::vec2(1.0f, 0.0f) },
		{ HUDPosition::RightDown, glm::vec2(1.0f, 1.0f) },
		{ HUDPosition::CenterLeft, glm::vec2(0.0f, 0.5f) },
		{ HUDPosition::LeftUp, glm::vec2(0.0f, 0.0f) },
		{ HUDPosition::LeftDown, glm::vec2(0.0f, 1.0f) },
	};
}



Jotar::HUDComponent::HUDComponent(GameObject* owner, HUDPosition position, glm::vec2 offset)
    : Component(owner)
    //, m_pTextureComponent{}
    , m_Position{ position }
    , m_Offset{ offset }
{
	CalculateScreenPos();
}

void Jotar::HUDComponent::Start()
{
   // m_pTextureComponent = GetOwner()->GetComponent<TextureComponent>();
	Renderer::GetInstance().AddObserver(this);

}

void Jotar::HUDComponent::OnNotify(const WindowResizeEvent& )
{
    CalculateScreenPos();
}

void Jotar::HUDComponent::SetPosition(HUDPosition position, glm::vec2 offset)
{
    m_Position = position;
    m_Offset = offset;
    CalculateScreenPos();
}


void Jotar::HUDComponent::CalculateScreenPos()
{
    glm::ivec2 size = GetOwner()->GetTransform()->GetSize();


    auto& renderer = Renderer::GetInstance();
    int windowWidth, windowHeight;
    SDL_GetWindowSize(renderer.GetGlSDLManager()->GetWindow(), &windowWidth, &windowHeight);

    glm::vec2 screenSize = glm::vec2(windowWidth, windowHeight);
    glm::vec2 position = CalculatePosition(m_Position, size, screenSize) + m_Offset;


    GetOwner()->GetTransform()->SetPosition(position.x, position.y);
}


glm::vec2 Jotar::HUDComponent::CalculatePosition(HUDPosition position, glm::ivec2 , glm::vec2 screenSize)
{
	auto it = s_PositionOffsets.find(position);
	if (it != s_PositionOffsets.end())
	{
		glm::vec2 normalizedPos = it->second;
		glm::vec2 absolutePos = glm::vec2(normalizedPos.x * screenSize.x, normalizedPos.y * screenSize.y);


		return absolutePos;
	}
	return glm::vec2(0.0f, 0.0f);
}