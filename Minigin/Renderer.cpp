#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "GLSDLManager.h"
#include "glad\glad.h"

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

void dae::Renderer::Init()
{
	SDL_Window* window = GLSDLManager::GetInstance().GetSDLWindow();


	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	ImguiWindowProperties properties{};

	//m_ImguiRenderer = std::make_unique<ImguiRenderer>();

	ImguiRenderer::GetInstance().Init(window, properties);

	SetWireFrameOn(false);
}

void dae::Renderer::Render() const
{

	// open gl ///////////////////////////

	//BeginRender();


	//SceneManager::GetInstance().Render();


	//EndRender();




	//SDL RENDER //////////////////



	SDL_RenderClear(m_Renderer);
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	

	SceneManager::GetInstance().Render();
	//m_ImguiRenderer->Render();

	//SDL_RenderPresent(m_Renderer);

}

void dae::Renderer::Destroy()
{
	//m_ImguiRenderer->Destroy();

	//SDL_DestroyTexture(m_GameTexture);

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::BeginRender() const
{
	Clear();
}

void dae::Renderer::EndRender() const
{
	SDL_RenderPresent(m_Renderer);
	//SDL_GL_SwapWindow(GLSDLManager::GetInstance().GetSDLWindow());
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	//SDL_SetRenderTarget(m_Renderer, m_GameTexture);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	//SDL_SetRenderTarget(m_Renderer, m_GameTexture);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_Renderer; }

void dae::Renderer::SetWireFrameOn(bool on)
{
	if (on)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}



void dae::Renderer::Clear() const
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
