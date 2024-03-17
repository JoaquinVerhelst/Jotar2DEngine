#pragma once
#include <glm/glm.hpp>


namespace Jotar
{
	enum class KeyboardButton;


	class KeyboardInput
	{
		class KeyboardInputImpl;
		KeyboardInputImpl* pImpl;

	public:
		KeyboardInput();
		~KeyboardInput();

		void Update();

		glm::ivec2 GetMouseLocation();
		bool IsMouseButtonDown();
		bool IsMouseButtonUp();

		bool IsKeyUp(KeyboardButton button);
		bool IsKeyDown(KeyboardButton button);
		bool IsKeyPressed(KeyboardButton button);


		bool GetIsGameQuit();
		void ClearFrameEvents();
	};
}