#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "GLSDLManager.h"
#include "glad\glad.h"

#include "WorldGrid.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Jotar::Renderer::Init()
{
	m_GLSDLManager = std::make_unique<GLSDLManager>();

	SDL_Window* window = m_GLSDLManager->GetWindow();


	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//ImguiWindowProperties properties{};

	//m_ImguiRenderer = std::make_unique<ImguiRenderer>();

	//ImguiRenderer::GetInstance().Init(window, properties);

	//SetWireFrameOn(false);
}

void Jotar::Renderer::Render() const
{

	// open gl testing ///////////////////////////

	//BeginRender();


	//SceneManager::GetInstance().Render();


	//EndRender();




	//SDL RENDER //////////////////




	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);


	WorldGrid::GetInstance().Render();
	SceneManager::GetInstance().Render();
	//m_ImguiRenderer->Render();

	SDL_RenderPresent(m_Renderer);

}

void Jotar::Renderer::Destroy()
{
	//m_ImguiRenderer->Destroy();

	//SDL_DestroyTexture(m_GameTexture);

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}

	m_GLSDLManager->Destroy();
}

void Jotar::Renderer::BeginRender() const
{
	Clear();
}

void Jotar::Renderer::EndRender() const
{
	//SDL_GL_SwapWindow(m_GLSDLManager->GetWindow());
}



void Jotar::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	//SDL_SetRenderTarget(m_Renderer, m_GameTexture);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Jotar::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	//SDL_SetRenderTarget(m_Renderer, m_GameTexture);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}


void Jotar::Renderer::RenderTexture(const Texture2D& texture, const glm::ivec4& src, const glm::ivec4& dst) const
{
	SDL_Rect dstRect{ dst.x, dst.y, dst.w, dst.a };
	SDL_Rect srcRect{ src.x, src.y, src.w, src.a };


	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dstRect);
}



SDL_Renderer* Jotar::Renderer::GetSDLRenderer() const { return m_Renderer; }

Jotar::GLSDLManager* Jotar::Renderer::GetGlSDLManager()
{
	return m_GLSDLManager.get();
}

void Jotar::Renderer::SetViewportPos(SDL_Rect newviewportRect)
{
	m_ViewPortRect = newviewportRect;
	//SDL_RenderSetViewport(m_Renderer, 
}

//void Jotar::Renderer::SetWireFrameOn(bool on)
//{
//	if (on)
//	{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	}
//	else
//	{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	}
//}



void Jotar::Renderer::Clear() const
{
	//glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
