#pragma once

#include "Singleton.h"
#include <memory>
#include "Command.h"
#include <vector>
#include "ControllerInput.h"
#include "KeyboardInput.h"
#include <utility>





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
	// TO DO, if time, find a beter way to do this, maybe somewhere else? using SDL_SCANCODE_TO_KEYCODE() ?
	enum class KeyboardButton
	{


		Key_Return = 13,
		Key_Backscpace = 8,
		Key_Escape = 27,



		Key_a = 97,
		Key_b = 98,
		Key_c = 99,
		Key_d = 100,
		Key_e = 101,
		Key_f = 102,
		Key_g = 103,
		Key_h = 104,
		Key_i = 105,
		Key_j = 106,
		Key_k = 107,
		Key_l = 108,
		Key_m = 109,
		Key_n = 110,
		Key_o = 111,
		Key_p = 112,
		Key_q = 113,
		Key_r = 114,
		Key_s = 115,
		Key_t = 116,
		Key_u = 117,
		Key_v = 118,
		Key_w = 119,
		Key_x = 120,
		Key_y = 121,
		Key_z = 122,



		key_LShift = 225,

		Key_LBracket = 91,
		Key_RBracket = 93,
		Key_Plus = 43,
		key_Minus = 45,

		key_F1 = 1073741882,
		key_F2 = 1073741883,
		key_F3 = 1073741884,
		key_F4 = 1073741885,
		key_F5 = 1073741886,
		key_F6 = 1073741887,
		key_F7 = 1073741888,
		key_F8 = 1073741889,
		key_F9 = 1073741890,
		key_F10 = 1073741891,
		key_F11 = 1073741892,
		key_F12 = 1073741893,



		
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
		void AddGeneralKeyBinding(KeyboardKey key, std::unique_ptr<Command> pCommand);
		void AddPlayerKeyBinding(KeyboardKey key, std::unique_ptr<Command> pCommand);




		void ClearPlayerInputBindings();

		glm::ivec2 GetMouseLocation() const;
		bool IsMouseButtonDown() const;
		bool IsMouseButtonUp() const;


		bool IsKeyUp(KeyboardButton key);
		bool IsControllerButtonUp(ControllerButton button);

		std::string GetPressedKeyString();

	private :

		void ControllersHandler() const;
		void KeyboardHandler() const;

		bool IsGameQuit() const;



		std::vector<std::pair<ControllerKey, std::unique_ptr<Command>>> m_pControllerBinds{};

		std::vector<std::pair<KeyboardKey, std::unique_ptr<Command>>> m_pGeneralKeyboardBinds{};
		std::vector<std::pair<KeyboardKey, std::unique_ptr<Command>>> m_pPlayerKeyboardBinds{};


		std::vector<std::unique_ptr<ControllerInput>> m_pControllers{};
		std::unique_ptr<KeyboardInput> m_KeyboardInput;
	};



	
}
