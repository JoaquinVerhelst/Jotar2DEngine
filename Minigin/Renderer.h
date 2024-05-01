#pragma once
#include "Singleton.h"
#include "ImguiRenderer.h"
#include "SDL.h"
#include <memory>
#include <stack>
#include "GLSDLManager.h"
#include "glm/glm.hpp"

namespace Jotar
{
	class Texture2D;

	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		//friend class PushFrameBuffer;
		//friend class PopFrameBuffer;

	public:
		void Init();
		void Render() const;
		void Destroy();


		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, const glm::ivec4& src, const glm::ivec4& dst) const;

		SDL_Renderer* GetSDLRenderer() const;
		GLSDLManager* GetGlSDLManager();


		void SetViewportPos(SDL_Rect newviewportRect);
		//void SetWireFrameOn(bool on);
		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

		void EndRender() const;
	private:


		void Clear() const;
		void BeginRender() const;
	

		std::unique_ptr<GLSDLManager> m_GLSDLManager;


		SDL_Renderer* m_Renderer{};
		SDL_Color m_ClearColor{};

		SDL_Rect m_ViewPortRect{0,0, 720,1080};

		//std::unique_ptr<ImguiRenderer> m_ImguiRenderer;
	};
}

