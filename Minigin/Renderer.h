#pragma once
#include "Singleton.h"
#include "ImguiRenderer.h"
#include "SDL.h"
#include <memory>
#include <stack>
#include "FrameBuffer.h"

namespace Jotar
{
	class Texture2D;

	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		friend class PushFrameBuffer;
		friend class PopFrameBuffer;

	public:
		void Init();
		void Render() const;
		void Destroy();


		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		void SetWireFrameOn(bool on);

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

		void EndRender() const;
	private:


		void Clear() const;

		void BeginRender() const;
	


		SDL_Renderer* m_Renderer{};
		SDL_Color m_ClearColor{};

		std::unique_ptr<ImguiRenderer> m_ImguiRenderer;
	};
}

