#pragma once

//#include "Singleton.h"
#include "SDL.h"
#include <memory>
#include "glm/glm.hpp"

namespace Jotar
{
	class SDLManager /*: public Singleton<GLSDLManager>*/
	{
	public:
		SDLManager();
		void Init();
		void Update();

		SDL_Window* GetWindow();
		
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