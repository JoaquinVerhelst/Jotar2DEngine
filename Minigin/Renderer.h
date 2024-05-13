#pragma once
#include "Singleton.h"
#include "ImguiRenderer.h"
#include "SDL.h"
#include <memory>
#include <stack>
#include "SDLManager.h"
#include "glm/glm.hpp"

#include "Subject.h"
#include "WindowEvents.h"

namespace Jotar
{
	class Texture2D;

	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{

	public:
		void Init();
		void Render() const;
		void Destroy();
		void Update();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, const glm::ivec4& src, const glm::ivec4& dst) const;

		SDL_Renderer* GetSDLRenderer() const;
		SDLManager* GetGlSDLManager();

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

		void EndRender() const;


		void AddObserver(Observer<WindowResizeEvent>* pObserver)
		{
			m_pSubject->AddObserver(pObserver);
		}
		void RemoveObserver(Observer<WindowResizeEvent>* pObserver)
		{
			m_pSubject->RemoveObserver(pObserver);
		}


	private:

		std::unique_ptr<Subject<WindowResizeEvent>> m_pSubject;

		void Clear() const;
		void BeginRender() const;
	

		std::unique_ptr<SDLManager> m_SDLManager;


		SDL_Renderer* m_Renderer{};
		SDL_Color m_ClearColor{};

		SDL_Rect m_ViewPortRect{0,0, 720,1080};

	};
}

