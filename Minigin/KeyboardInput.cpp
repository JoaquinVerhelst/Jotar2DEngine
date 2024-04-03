#include "KeyboardInput.h"
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <SDL.h>

namespace Jotar
{

	//  ===============================================================================  //
	//  						KeyboardInput Impl Class								 //
	//  ===============================================================================  //


	class KeyboardInput::KeyboardInputImpl final
	{
	public:

		KeyboardInputImpl()
		{
		
		
		};
		~KeyboardInputImpl() = default;

		void Update();

		glm::ivec2 GetMouseLocation() const;
		bool IsMouseButtonUsed(SDL_EventType inputType) const;
		bool IsKeyUsed(SDL_KeyCode key, SDL_EventType inputType) const;
		bool IsKeyPressed(SDL_KeyCode key) const;

		void ClearFrameEvents();

		bool GetIsGameQuit() const;

	private:
		std::vector<SDL_Event> m_SDLFrameKeyEvents;
		std::vector<SDL_Event> m_SDLFrameMouseEvents;
		bool m_IsGameQuit = false;
	};


	//  ===============================================================================  //
	//  				KeyboardInput Impl Functions Implementation						 //
	//  ===============================================================================  //


	void KeyboardInput::KeyboardInputImpl::Update()
	{
		SDL_Event e;

		while (SDL_PollEvent(&e) != 0)
		{
			// There are 13 SDL_Event types, Not worth to save each 13 types when I only need these
			if (e.type == SDL_EventType::SDL_KEYUP 
				|| e.type == SDL_EventType::SDL_KEYDOWN
				|| e.type == SDL_EventType::SDL_QUIT)
				m_SDLFrameKeyEvents.emplace_back(e);

			if (e.type == SDL_EventType::SDL_MOUSEBUTTONDOWN 
				|| e.type == SDL_EventType::SDL_MOUSEBUTTONUP
				|| e.type == SDL_EventType::SDL_MOUSEMOTION)
				m_SDLFrameMouseEvents.emplace_back(e);

			//ImGui_ImplSDL2_ProcessEvent(&e);
		}

		for (size_t i = 0; i < m_SDLFrameKeyEvents.size(); i++)
		{
			if (m_SDLFrameKeyEvents[i].type == SDL_QUIT) {
				m_IsGameQuit = true;
			}
		}
	}

	glm::ivec2 KeyboardInput::KeyboardInputImpl::GetMouseLocation() const
	{
		for (size_t i = 0; i < m_SDLFrameMouseEvents.size(); i++)
		{
			if (m_SDLFrameMouseEvents[i].type == SDL_MOUSEMOTION)
			{
				return glm::ivec2{ (int)m_SDLFrameMouseEvents[i].motion.x,(int)m_SDLFrameMouseEvents[i].motion.y };
			}
		}

		return glm::ivec2{ 0,0 };
	}

	bool KeyboardInput::KeyboardInputImpl::IsMouseButtonUsed(SDL_EventType inputType) const
	{
		for (size_t i = 0; i < m_SDLFrameMouseEvents.size(); i++)
		{
			if (m_SDLFrameMouseEvents[i].type == static_cast<unsigned>(inputType))
			{
				return true;
			}
		}

		return false;
	}

	bool KeyboardInput::KeyboardInputImpl::IsKeyUsed(SDL_KeyCode key, SDL_EventType inputType) const
	{
		for (size_t i = 0; i < m_SDLFrameKeyEvents.size(); i++)
		{
			if (m_SDLFrameKeyEvents[i].type == static_cast<unsigned>(inputType))
			{
				if (m_SDLFrameKeyEvents[i].key.keysym.sym == key)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool KeyboardInput::KeyboardInputImpl::IsKeyPressed(SDL_KeyCode key) const
	{
		SDL_Scancode scanCode = SDL_GetScancodeFromKey(key);

		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		if (keystate[scanCode])
		{
			return true;
		}

		return false;
	}

	void KeyboardInput::KeyboardInputImpl::ClearFrameEvents()
	{
		m_SDLFrameKeyEvents.clear();
		m_SDLFrameMouseEvents.clear();
	}

	bool KeyboardInput::KeyboardInputImpl::GetIsGameQuit() const
	{
		return m_IsGameQuit;
	}


	//  ===============================================================================  //
	//  						KeyboardInput Functions									 //
	//  ===============================================================================  //

	Jotar::KeyboardInput::KeyboardInput()
	{
		pImpl = new KeyboardInputImpl();
	}

	KeyboardInput::~KeyboardInput()
	{
		delete pImpl;
	}

	void KeyboardInput::Update()
	{
		pImpl->Update();
	}


	glm::ivec2 KeyboardInput::GetMouseLocation() const
	{
		return pImpl->GetMouseLocation();
	}

	bool KeyboardInput::IsMouseButtonDown() const
	{
		return pImpl->IsMouseButtonUsed(SDL_EventType::SDL_MOUSEBUTTONDOWN);
	}

	bool KeyboardInput::IsMouseButtonUp() const
	{
		return pImpl->IsMouseButtonUsed(SDL_EventType::SDL_MOUSEBUTTONUP);
	}


	bool KeyboardInput::IsKeyUp(KeyboardButton button) const
	{
		return pImpl->IsKeyUsed(static_cast<SDL_KeyCode>(button), SDL_EventType::SDL_KEYUP);
	}

	bool KeyboardInput::IsKeyDown(KeyboardButton button) const
	{
		return pImpl->IsKeyUsed(static_cast<SDL_KeyCode>(button), SDL_EventType::SDL_KEYDOWN);
	}

	bool KeyboardInput::IsKeyPressed(KeyboardButton button) const
	{
		return pImpl->IsKeyPressed(static_cast<SDL_KeyCode>(button));
	}

	bool KeyboardInput::GetIsGameQuit() const
	{
		return pImpl->GetIsGameQuit();
	}

	void KeyboardInput::ClearFrameEvents()
	{
		pImpl->ClearFrameEvents();
	}

}