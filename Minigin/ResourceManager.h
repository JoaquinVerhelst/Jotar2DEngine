#pragma once
#include <string>
#include <memory>
#include "Singleton.h"
#include <vector>
#include "Enums.h"


namespace Jotar
{
	class Texture2D;
	class Font;

	struct SharedTexture2D
	{
		SharedTexture2D(std::shared_ptr<Texture2D> texture, std::string name)
			:Texture{texture}
			, Name{name}
		{
		}
		std::shared_ptr<Texture2D> Texture;
		std::string Name;
	};

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		void AddSharedTexture(const std::string& file, const std::string& name);
		void AddSharedSpriteSheet(const std::string& file, SpriteSheet spriteSheet, const std::string& name);

		SpriteSheet& GetSharedSpriteSheet(std::string spriteSheetName);
		std::shared_ptr<Texture2D> GetSharedTexture(std::string textureName);

		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;


		std::vector<SharedTexture2D> m_SharedTextures;
		std::vector<SpriteSheet> m_SharedSpriteSheets;


	};
}
