#include "BaseTextureComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "HUDComponent.h"

Jotar::BaseTextureComponent::BaseTextureComponent(GameObject* owner)
	: Component(owner)
{
}

void Jotar::BaseTextureComponent::Start()
{
	if (GetOwner()->HasComponent<HUDComponent>())
		Renderer::GetInstance().AddHUDToRender(this);
	else
		Renderer::GetInstance().AddTextureToRender(this);
}

void Jotar::BaseTextureComponent::Reset()
{
	Renderer::GetInstance().RemoveTextureToRender(this);
}

void Jotar::BaseTextureComponent::OnDestroy()
{
	Reset();
}
