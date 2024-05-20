#include "BaseTextureComponent.h"
#include "Renderer.h"


Jotar::BaseTextureComponent::BaseTextureComponent(GameObject* owner)
	: Component(owner)
{
}

void Jotar::BaseTextureComponent::Start()
{
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
