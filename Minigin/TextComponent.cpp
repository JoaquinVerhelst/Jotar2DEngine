#include "TextComponent.h"

#include "Renderer.h"
#include "GameObject.h"
#include "Font.h"
#include <SDL_ttf.h>
#include "WorldTimeManager.h"
#include "Texture2D.h"


dae::TextComponent::TextComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, bool needUpdating)
	: Component(owner),
	m_NeedsUpdate(needUpdating),
	m_Text(text),
	m_Font(font),
	m_DoOnce{ true },
	m_Color{color}
{
	UpdateText();
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		UpdateText();
	}
}

void dae::TextComponent::Render() const
{
	const auto& pos = GetOwner()->GetPosition().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void dae::TextComponent::SetText(const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating)
{
	m_Text = text;
	m_Font = font;
	m_NeedsUpdate = needUpdating;
	UpdateText();
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	UpdateText();
}

void dae::TextComponent::UpdateText()
{
	if (m_Text.empty()) return;


	const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_Texture = std::make_unique<Texture2D>(texture);
}
