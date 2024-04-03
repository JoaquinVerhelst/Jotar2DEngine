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
{
	m_SpriteSheet.pTexture = std::make_unique<Texture2D>(filePath);

	m_SpriteSheet.TotalColumns = columns;
	m_SpriteSheet.TotalRows = rows;

	m_SpriteSheet.ClipHeight = m_SpriteSheet.pTexture->GetSize().y / columns;
	m_SpriteSheet.ClipWidth = m_SpriteSheet.pTexture->GetSize().x / rows;


	m_SrcRect.w = m_SpriteSheet.ClipWidth;
	m_SrcRect.a = m_SpriteSheet.ClipHeight;

	m_SrcRect.y = m_SpriteSheet.CurrentColumn * m_SpriteSheet.ClipHeight;
	m_SrcRect.x = m_SpriteSheet.CurrentRow * m_SpriteSheet.ClipWidth;
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
	glm::ivec4 dst{};
	dst.x = static_cast<int>(pos.x);
	dst.y = static_cast<int>(pos.y);
	dst.w = m_SrcRect.w;
	dst.a = m_SrcRect.a;


	Renderer::GetInstance().RenderTexture(*m_SpriteSheet.pTexture, m_SrcRect, dst);
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
