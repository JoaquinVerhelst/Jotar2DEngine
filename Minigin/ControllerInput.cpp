#include "ControllerInput.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "InputManager.h"


namespace Jotar
{

	//  ===============================================================================  //
	//  						ControllerInput	Impl									 //
	//  ===============================================================================  //

	class ControllerInput::ControllerInputImpl final
	{
	public:
		ControllerInputImpl(unsigned int controllerID)
			: m_ControllerID{ controllerID }
		{}

		~ControllerInputImpl() = default;


		void Update()
		{
			std::swap(m_PreviousState, m_CurrentState);
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

			XInputGetState(m_ControllerID, &m_CurrentState);

			auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		}

		bool IsDownThisFrame(unsigned int button) const
		{
			return m_ButtonsPressedThisFrame & button;
		}

		bool IsUpThisFrame(unsigned int button) const
		{
			return m_ButtonsReleasedThisFrame & button;
		}

		bool IsPressed(unsigned int button) const
		{
			return m_CurrentState.Gamepad.wButtons & button;
		}

		unsigned int GetMaxControllers() const
		{
			return XUSER_MAX_COUNT;
		}


	private:

		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};

		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};

		unsigned int m_ControllerID;
	};



	//  ===============================================================================  //
	//  							InputController										 //
	//  ===============================================================================  //

	ControllerInput::ControllerInput(unsigned int controllerID)
	{
		pImpl = new ControllerInputImpl(controllerID);
	}

	ControllerInput::~ControllerInput()
	{
		delete pImpl;
	}

	void ControllerInput::Update()
	{
		return pImpl->Update();
	}

	bool ControllerInput::IsPressed(ControllerButton button) const
	{
		return pImpl->IsPressed(static_cast<unsigned int>(button));
	}

	bool ControllerInput::IsDown(ControllerButton button) const
	{
		return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
	}

	bool ControllerInput::IsUp(ControllerButton button) const
	{
		return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
	}

	unsigned int ControllerInput::GetMaxControllers() const
	{
		return pImpl->GetMaxControllers();
	}

}