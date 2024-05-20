#include "TextureComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "WorldTimeManager.h"



Jotar::TextureComponent::TextureComponent(GameObject* owner, const std::string& filePath, bool isStatic, int columns, int rows)
	: BaseTextureComponent(owner)
	, m_AnimTime{}
	, m_AnimFrame{}
	, m_SpriteSheet{}
	, m_IsStatic{ isStatic }
	, m_NrFramesPerSec{ 3 }
{
	m_SpriteSheet.m_pTexture = std::make_shared<Texture2D>(filePath);
	InitSpriteSheet(columns, rows);
	Initizialize();
}

Jotar::TextureComponent::TextureComponent(GameObject* owner, std::shared_ptr<Texture2D> texture)
	: BaseTextureComponent(owner)
	, m_AnimTime{}
	, m_AnimFrame{}
	, m_SpriteSheet{}
	, m_IsStatic{ true }
	, m_NrFramesPerSec{ 3 }
{
	m_SpriteSheet.m_pTexture = texture;
	InitSpriteSheet(1, 1);
	Initizialize();
}

Jotar::TextureComponent::TextureComponent(GameObject* owner, SpriteSheet& spriteSheet, int currentRow, int currentColumn)
	: BaseTextureComponent(owner)
	, m_AnimTime{}
	, m_AnimFrame{}
	, m_SpriteSheet{}
	, m_IsStatic{ false }
	, m_NrFramesPerSec{ 3 }
{
	m_SpriteSheet = spriteSheet;
	m_SpriteSheet.CurrentColumn = currentColumn;
	m_SpriteSheet.CurrentRow = currentRow;


	Initizialize();
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
	dst.x = static_cast<int>(pos.x) - m_Size.x / 2;
	dst.y = static_cast<int>(pos.y) - m_Size.y / 2;
	dst.w = m_Size.x;
	dst.z = m_Size.y;

	Renderer::GetInstance().RenderTexture(*m_SpriteSheet.m_pTexture, m_SrcRect, dst);
}

void Jotar::TextureComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_SpriteSheet.m_pTexture = texture;
	InitSpriteSheet(1, 1);
	Initizialize();
}

void Jotar::TextureComponent::SetDestroyOnLastFrame(bool newValue)
{
	m_DestroyOnLastFrame = newValue;
}

void Jotar::TextureComponent::SetCurrentRow(int currentRow)
{
	m_SpriteSheet.CurrentRow = currentRow;
}

void Jotar::TextureComponent::SetCurrentColumn(int currentColumn)
{
	m_SpriteSheet.CurrentColumn = currentColumn;
}


void Jotar::TextureComponent::SetSize(glm::ivec2 size)
{
	m_Size = size;
}

void Jotar::TextureComponent::SetSelectedFrames(glm::ivec2 startAndEndFrames)
{
	m_SelectedFramesIndexes = startAndEndFrames;

	if (m_SpriteSheet.Direction == SpriteSheet::SpriteSheetDirection::Right)
		m_SpriteSheet.CurrentRow = m_SelectedFramesIndexes.x;
	else
		m_SpriteSheet.CurrentColumn = m_SelectedFramesIndexes.y;

}

void Jotar::TextureComponent::UpdateFrame()
{
	m_AnimTime += WorldTimeManager::GetInstance().GetDeltaTime();

	float frameTime = 1.f / m_NrFramesPerSec;

	if (m_AnimTime > frameTime)
	{
		if (m_SpriteSheet.Direction == SpriteSheet::SpriteSheetDirection::Right)
		{
			if (m_DestroyOnLastFrame)
				CheckForDestroy(m_SpriteSheet.CurrentRow, m_SpriteSheet.TotalRows - 1);

			++m_SpriteSheet.CurrentRow;

			if (m_SpriteSheet.CurrentRow > m_SelectedFramesIndexes.y)
				m_SpriteSheet.CurrentRow = m_SelectedFramesIndexes.x;
			
			m_SrcRect.x = m_SpriteSheet.CurrentRow * m_SpriteSheet.ClipWidth;
			m_SrcRect.y = m_SpriteSheet.CurrentColumn * m_SpriteSheet.ClipHeight;
		}
		else
		{
			if (m_DestroyOnLastFrame)
				CheckForDestroy(m_SpriteSheet.CurrentColumn, m_SpriteSheet.TotalColumns - 1);

			++m_SpriteSheet.CurrentColumn;
			if (m_SpriteSheet.CurrentColumn > m_SelectedFramesIndexes.y)
				m_SpriteSheet.CurrentColumn = m_SelectedFramesIndexes.x;

			m_SrcRect.y = m_SpriteSheet.CurrentColumn * m_SpriteSheet.ClipHeight;
			m_SrcRect.x = m_SpriteSheet.CurrentRow * m_SpriteSheet.ClipWidth;
		}

		m_AnimTime -= frameTime;
	}
}

void Jotar::TextureComponent::InitSpriteSheet(int columns, int rows)
{
	m_SpriteSheet.TotalColumns = columns;
	m_SpriteSheet.TotalRows = rows;
				 
	m_SpriteSheet.ClipHeight = m_SpriteSheet.m_pTexture->GetSize().y / columns;
	m_SpriteSheet.ClipWidth = m_SpriteSheet.m_pTexture->GetSize().x / rows;
}

void Jotar::TextureComponent::CheckForDestroy(int currentFrame, int totalFrames)
{
	if (currentFrame == totalFrames)
	{
		GetOwner()->Destroy();
	}
}

void Jotar::TextureComponent::Initizialize()
{
	glm::vec2 size = GetOwner()->GetTransform()->GetSize();
	m_Size = size;

	m_SrcRect.z = m_SpriteSheet.ClipHeight;
	m_SrcRect.w = m_SpriteSheet.ClipWidth;
					   
	m_SrcRect.y = m_SpriteSheet.CurrentColumn * m_SpriteSheet.ClipHeight;
	m_SrcRect.x = m_SpriteSheet.CurrentRow * m_SpriteSheet.ClipWidth;

	if (m_SpriteSheet.Direction == SpriteSheet::SpriteSheetDirection::Right)
		m_SelectedFramesIndexes = { 0, m_SpriteSheet.TotalRows };
	else
		m_SelectedFramesIndexes = { 0, m_SpriteSheet.TotalColumns };
}

void Jotar::TextureComponent::SetAnimationSpeedInNrOfFramesPerSecond(int animSpeed)
{
	m_NrFramesPerSec = animSpeed;
}