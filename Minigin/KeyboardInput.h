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

		glm::ivec2 GetMouseLocation() const;
		bool IsMouseButtonDown() const;
		bool IsMouseButtonUp() const;

		bool IsKeyUp(KeyboardButton button) const;
		bool IsKeyDown(KeyboardButton button) const;
		bool IsKeyPressed(KeyboardButton button) const;


		bool GetIsGameQuit() const;
		void ClearFrameEvents();
	};
}