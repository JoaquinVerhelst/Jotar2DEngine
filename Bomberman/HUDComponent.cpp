#include "HUDComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "TextureComponent.h"

Jotar::HUDComponent::HUDComponent(GameObject* owner)
	: Component(owner)
    , m_pTextureComponent{}
{

}

void Jotar::HUDComponent::Start()
{
    m_pTextureComponent = GetOwner()->GetComponent<TextureComponent>();
	Renderer::GetInstance().AddObserver(this);
    CalculateScreenPos();
}

void Jotar::HUDComponent::OnNotify(const WindowResizeEvent& )
{
    CalculateScreenPos();
}

void Jotar::HUDComponent::CalculateScreenPos()
{
    glm::ivec2 size = GetOwner()->GetTransform()->GetSize();

    // Calculate the center of the screen after resizing
    auto& renderer = Renderer::GetInstance();
    int windowWidth, windowHeight;
    SDL_GetWindowSize(renderer.GetGlSDLManager()->GetWindow(), &windowWidth, &windowHeight);
    glm::vec2 screenCenter = glm::vec2(windowWidth / 2, windowHeight / 2);

    // Update the position of the texture to ensure it stays in the middle of the screen
    GetOwner()->GetTransform()->SetPosition(screenCenter.x, static_cast<float>(0 + size.y / 2));
}
