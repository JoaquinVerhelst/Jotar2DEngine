#pragma once

#include "Singleton.h"
#include "SDL.h"

#include <memory>


namespace dae
{
	class FrameBuffer;

	// TODO: I Will probs refactor this and/or move this code somehwre else or rename this 
	class GLSDLManager : public Singleton<GLSDLManager>
	{
	public:

		void Init();


		SDL_Window* GetSDLWindow();
		void Destroy();

	private:
		void PrintSDLVersion();


		SDL_Window* m_Window{};
		SDL_GLContext m_GLContext{};

		// todo, change the init loc of the window size
		int m_WindowWidth{1080};
		int m_WindowHeight{720};
	};

}