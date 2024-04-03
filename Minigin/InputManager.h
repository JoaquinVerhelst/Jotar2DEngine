#pragma once

#include "Singleton.h"
#include <memory>
#include "Command.h"
#include <vector>
#include "ControllerInput.h"
#include "KeyboardInput.h"

namespace Jotar
{


#pragma region Buttons

	enum class ControllerButton
	{
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRight = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000
	};

	// its based on SDL_KeyCode and Not Sdl_Scancode
	enum class KeyboardButton
	{

		Key_a = 'a',
		Key_b = 'b',
		Key_c = 'c',
		Key_d = 'd',
		Key_e = 'e',
		Key_f = 'f',
		Key_g = 'g',
		Key_h = 'h',
		Key_i = 'i',
		Key_j = 'j',
		Key_k = 'k',
		Key_l = 'l',
		Key_m = 'm',
		Key_n = 'n',
		Key_o = 'o',
		Key_p = 'p',
		Key_q = 'q',
		Key_r = 'r',
		Key_s = 's',
		Key_t = 't',
		Key_u = 'u',
		Key_v = 'v',
		Key_w = 'w',
		Key_x = 'x',
		Key_y = 'y',
		Key_z = 'z',
		
		Key_RIGHT = 1073741903,
		Key_LEFT = 1073741904,
		Key_DOWN = 1073741905,
		Key_UP = 1073741906,
	};

#pragma endregion


	enum class InputType {
		Pressed,
		Up,
		Down
	};

	struct KeyboardKey
	{
		KeyboardKey(KeyboardButton button, InputType inputType = InputType::Pressed)
			: button{ button }
			, inputType{ inputType }
		{}

		InputType inputType;
		KeyboardButton button;
	};

	struct ControllerKey
	{
		ControllerKey(unsigned int controllerID, ControllerButton cButton, InputType inputType = InputType::Pressed)
			: controllerID{ controllerID }
			, button{ cButton }
			, inputType{ inputType }
		{}

		unsigned int controllerID;
		ControllerButton button;
		InputType inputType;
	};



	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		

		bool ProcessInput() const;
		void ClearFrameEvents();

		void AddControllerBinding(ControllerKey key, std::unique_ptr<Command> pCommand);
		void AddKeyBinding(KeyboardKey key, std::unique_ptr<Command> pCommand);


		glm::ivec2 GetMouseLocation() const;
		bool IsMouseButtonDown() const;
		bool IsMouseButtonUp() const;



	private :

		void ControllersHandler() const;
		void KeyboardHandler() const;

		bool IsGameQuit() const;



		std::vector<std::pair<ControllerKey, std::unique_ptr<Command>>> m_pControllerBinds{};
		std::vector<std::pair<KeyboardKey, std::unique_ptr<Command>>> m_pKeyboardBinds{};

		std::vector<std::unique_ptr<ControllerInput>> m_pControllers{};
		std::unique_ptr<KeyboardInput> m_KeyboardInput;
	};



	
}
