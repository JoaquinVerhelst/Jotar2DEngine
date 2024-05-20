#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "SDLManager.h"
#include <algorithm>


#include "BaseTextureComponent.h"

#include <iostream>

void Jotar::Renderer::Init()
{
	m_SDLManager = std::make_unique<SDLManager>();

	SDL_Window* window = m_SDLManager->GetWindow();


	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	m_pSubject = std::make_unique<Subject<WindowResizeEvent>>();
}

void Jotar::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	for (const auto& layer : m_LayeredTextures) {
		for (const auto& textureComponent : layer.second) {
			textureComponent->Render();
		}
	}

	SDL_RenderPresent(m_Renderer);
}

void Jotar::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}

	m_SDLManager->Destroy();
}

void Jotar::Renderer::Update()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED ||
				e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
				e.window.event == SDL_WINDOWEVENT_MAXIMIZED)
			{
				std::cout << "WINDOW EVENT" << '\n';

				int windowWidth, windowHeight;
				SDL_GetWindowSize(m_SDLManager->GetWindow(), &windowWidth, &windowHeight);

				float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

				float zoomFactor = 1.f;
				int logicalWidth = static_cast<int>(windowWidth * zoomFactor);
				int logicalHeight = static_cast<int>(windowHeight * zoomFactor);

				if (aspectRatio >= 1.0f) {
					logicalWidth = static_cast<int>(logicalHeight * aspectRatio);
				}
				else {
					logicalHeight = static_cast<int>(logicalWidth / aspectRatio);
				}

				SDL_RenderSetLogicalSize(m_Renderer, logicalWidth, logicalHeight);
				SDL_RenderSetScale(m_Renderer, 1.0f / zoomFactor, 1.0f / zoomFactor);

				// Ensure m_pSubject and NotifyObservers are valid and implemented correctly
				if (m_pSubject) {
					m_pSubject->NotifyObservers(WindowResizeEvent());
				}
				else {
					std::cerr << "m_pSubject is null" << '\n';
				}
			}
		}
	}
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
	SDL_Rect dstRect{ dst.x, dst.y, dst.w, dst.z };
	SDL_Rect srcRect{ src.x, src.y, src.w, src.z };


	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dstRect);
}

void Jotar::Renderer::AddTextureToRender(BaseTextureComponent* textureComponent)
{
	int layer = textureComponent->GetLayer();

	m_LayeredTextures[layer].emplace_back(textureComponent);

	std::sort(m_LayeredTextures[layer].begin(), m_LayeredTextures[layer].end(), [](const auto& a, const auto& b) {
		return a->GetLayer() < b->GetLayer();
	});
}

void Jotar::Renderer::RemoveTextureToRender(BaseTextureComponent* textureComponent)
{
	int layer = textureComponent->GetLayer();

	auto it = m_LayeredTextures.find(layer);
	if (it != m_LayeredTextures.end()) {
		// Find and remove the texture from the layer's vector
		auto& textures = it->second;
		textures.erase(std::remove_if(textures.begin(), textures.end(),
			[textureComponent](BaseTextureComponent* tc) {
				return tc == textureComponent;
			}), textures.end());

		// If the vector is empty after removal, erase the layer from the map
		if (textures.empty()) {
			m_LayeredTextures.erase(it);
		}
	}
}



SDL_Renderer* Jotar::Renderer::GetSDLRenderer() const { return m_Renderer; }

Jotar::SDLManager* Jotar::Renderer::GetGlSDLManager()
{
	return m_SDLManager.get();
}


void Jotar::Renderer::Clear() const
{
	//glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
