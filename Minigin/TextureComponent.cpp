#include "TextureComponent.h"
#include "Renderer.h"
#include "GameObject.h"

dae::TextureComponent::TextureComponent(GameObject* owner, const std::string& filePath)
	: Component(owner),
	m_Texture{ nullptr }
{
	m_Texture = std::make_unique<Texture2D>(filePath);
}


void dae::TextureComponent::SetTexture(const std::string& filePath)
{
	m_Texture->LoadTexture(filePath);
}


void dae::TextureComponent::Render() const
{
	const auto& pos = GetOwner()->GetTransform()->GetLocalPosition();

	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}
