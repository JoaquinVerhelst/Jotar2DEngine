#include "SDLManager.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdexcept>
#include "SDL.h"
#include "glad/glad.h"
#include "FrameBuffer.h"


#include <iostream>

Jotar::SDLManager::SDLManager()
{
	Init();
}

void Jotar::SDLManager::Init()
{

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_WindowWidth,
		m_WindowHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
}

void Jotar::SDLManager::Update()
{


}

SDL_Window* Jotar::SDLManager::GetWindow()
{
	return m_Window;
}


void Jotar::SDLManager::Destroy()
{
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}




void Jotar::SDLManager::PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
		printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}
