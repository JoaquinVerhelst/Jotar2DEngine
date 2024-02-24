#pragma once
#include <glm/vec2.hpp>
#include <string>


struct SDL_Texture;
namespace dae
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		explicit Texture2D(std::string filePath);
		~Texture2D();

		glm::ivec2 GetSize() const;
		void LoadTexture(std::string filePath);


		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_pTexture;

		float m_Width;
		float m_Height;
	};
}
