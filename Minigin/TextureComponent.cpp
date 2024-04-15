#include "TextureComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "WorldTimeManager.h"



Jotar::TextureComponent::TextureComponent(GameObject* owner, const std::string& filePath, bool isStatic, int columns, int rows)
	: Component(owner)
	, m_AnimTime{}
	, m_AnimFrame{}
	, m_SpriteSheet{}
	, m_IsStatic{ isStatic }
	, m_NrFramesPerSec{ 3 }
	, m_IsSharedResource{ false }
{
	m_SpriteSheet.pTexture = std::make_unique<Texture2D>(filePath);

	m_SpriteSheet.TotalColumns = columns;
	m_SpriteSheet.TotalRows = rows;

	m_SpriteSheet.ClipHeight = 32;// m_SpriteSheet.pTexture->GetSize().y / columns;
	m_SpriteSheet.ClipWidth = 32;// m_SpriteSheet.pTexture->GetSize().x / rows;


	m_SrcRect.w = m_SpriteSheet.ClipWidth;
	m_SrcRect.z = m_SpriteSheet.ClipHeight;

	m_SrcRect.y = m_SpriteSheet.CurrentColumn * m_SpriteSheet.ClipHeight;
	m_SrcRect.x = m_SpriteSheet.CurrentRow * m_SpriteSheet.ClipWidth;
}

Jotar::TextureComponent::TextureComponent(GameObject* owner, std::shared_ptr<Texture2D> texture)
	: Component(owner)
	, m_AnimTime{}
	, m_AnimFrame{}
	, m_SpriteSheet{}
	, m_IsStatic{ true }
	, m_NrFramesPerSec{ 0 }
	, m_IsSharedResource{ true}
	, m_pSharedTexture{ texture }
{
	m_SrcRect.w = texture->GetSize().x;
	m_SrcRect.z = texture->GetSize().y;
	m_SrcRect.y = m_SrcRect.z;
	m_SrcRect.x = m_SrcRect.w;
}

void Jotar::TextureComponent::Update()
{
	if (!m_IsStatic)
	{
		UpdateFrame();
	}
}

void Jotar::TextureComponent::Render() const
{
	const auto& pos = GetOwner()->GetTransform()->GetWorldPosition();

	if (!m_IsSharedResource)
	{
		glm::ivec4 dst{};
		dst.x = static_cast<int>(pos.x) - m_SpriteSheet.ClipWidth / 2;
		dst.y = static_cast<int>(pos.y) - m_SpriteSheet.ClipHeight / 2;
		dst.w = m_SrcRect.w;
		dst.z = m_SrcRect.z;

		Renderer::GetInstance().RenderTexture(*m_SpriteSheet.pTexture, m_SrcRect, dst);
	}
	else
		Renderer::GetInstance().RenderTexture(*m_pSharedTexture, pos.x - static_cast<float>(m_SrcRect.w / 2), pos.y - static_cast<float>(m_SrcRect.z / 2));
}

void Jotar::TextureComponent::SetSize(glm::ivec2 size)
{
	m_SrcRect.w = size.x;
	m_SrcRect.z = size.y;
}

void Jotar::TextureComponent::UpdateFrame()
{
	m_AnimTime += WorldTimeManager::GetInstance().GetDeltaTime();

	float frameTime = 1.f / m_NrFramesPerSec;

	if (m_AnimTime > frameTime)
	{
		if (m_SpriteSheet.Direction == SpriteSheet::SpriteSheetDirection::Right)
		{
			++m_SpriteSheet.CurrentRow %= m_SpriteSheet.TotalRows;
			m_SrcRect.x = m_SpriteSheet.CurrentRow * m_SpriteSheet.ClipWidth;
		}
		else
		{
			++m_SpriteSheet.CurrentColumn %= m_SpriteSheet.TotalColumns;
			m_SrcRect.y = m_SpriteSheet.CurrentColumn * m_SpriteSheet.ClipHeight;
		}

		m_AnimTime -= frameTime;
	}
}
