#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

void Jotar::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<Jotar::Texture2D> Jotar::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_dataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	return std::make_shared<Texture2D>(texture);
}

void Jotar::ResourceManager::AddSharedTexture(const std::string& file, const std::string& name)
{
	auto texture = LoadTexture(file);
	m_SharedTextures.emplace_back(SharedTexture2D(std::move(texture), name));
}

std::shared_ptr<Jotar::Texture2D> Jotar::ResourceManager::GetSharedTexture(std::string textureName)
{
	for (auto& sharedTexture : m_SharedTextures)
	{
		if (sharedTexture.Name == textureName)
			return sharedTexture.Texture;
	}

	throw std::runtime_error(std::string("Failed to find Shared Texture by this name: " + textureName));
}

std::shared_ptr<Jotar::Font> Jotar::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_dataPath + file, size);
}
