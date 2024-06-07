#include "InputManager.h"

#include <vector>
#include <stdexcept>
#include <iostream>


namespace Jotar
{
	Jotar::InputManager::InputManager()
	{
		m_KeyboardInput = std::make_unique<KeyboardInput>();
	}

	bool InputManager::ProcessInput() const
	{
		//Controllers Update
		for (const auto& controller : m_pControllers)
		{
			controller->Update();
		}

		m_KeyboardInput->Update();

		//Input bindings check
		ControllersHandler();

		KeyboardHandler();

		return !IsGameQuit();
	}

	void InputManager::ControllersHandler() const
	{
		for (const auto& controllerBind: m_pControllerBinds)
		{
			const auto& [key, command] = controllerBind;

			switch (key.inputType)
			{
			case InputType::Pressed:
				if (m_pControllers[key.controllerID]->IsPressed(key.button))
					command->Execute();
				break;
			case InputType::Up:
				if (m_pControllers[key.controllerID]->IsUp(key.button))
					command->Execute();
				break;
			case InputType::Down:
				if (m_pControllers[key.controllerID]->IsDown(key.button))
					command->Execute();
				break;
			}
		}
	}

	void InputManager::KeyboardHandler() const
	{
		for (const auto& keyboardBinds : m_pGeneralKeyboardBinds)
		{
			const auto& [key, command] = keyboardBinds;

			switch (key.inputType)
			{
			case InputType::Pressed:
				if (m_KeyboardInput->IsKeyPressed(key.button))
					command->Execute();
				break;
			case InputType::Up:
				if (m_KeyboardInput->IsKeyUp(key.button))
					command->Execute();
				break;
			case InputType::Down:
				if (m_KeyboardInput->IsKeyDown(key.button))
					command->Execute();
				break;
			}
		}

		for (const auto& keyboardBinds : m_pPlayerKeyboardBinds)
		{
			const auto& [key, command] = keyboardBinds;

			switch (key.inputType)
			{
			case InputType::Pressed:
				if (m_KeyboardInput->IsKeyPressed(key.button))
					command->Execute();
				break;
			case InputType::Up:
				if (m_KeyboardInput->IsKeyUp(key.button))
					command->Execute();
				break;
			case InputType::Down:
				if (m_KeyboardInput->IsKeyDown(key.button))
					command->Execute();
				break;
			}
		}
	}

	void InputManager::AddControllerBinding(ControllerKey key, std::unique_ptr<Command> pCommand)
	{
		if (key.controllerID >= m_pControllers.size())
		{
			if (!m_pControllers.empty())
			{
				if (key.controllerID >= m_pControllers[0]->GetMaxControllers())
				{
					throw std::runtime_error("The controller ID exceeded the maximum amount of controllers");
				}
			}
			m_pControllers.push_back(std::make_unique<ControllerInput>(key.controllerID));
		}

		m_pControllerBinds.emplace_back(std::pair(key, std::move(pCommand)));
	}

	void InputManager::AddGeneralKeyBinding(KeyboardKey key, std::unique_ptr<Command> pCommand)
	{
		m_pGeneralKeyboardBinds.emplace_back(std::pair(key, std::move(pCommand)));
	}

	void InputManager::AddPlayerKeyBinding(KeyboardKey key, std::unique_ptr<Command> pCommand)
	{
		m_pPlayerKeyboardBinds.emplace_back(std::pair(key, std::move(pCommand)));
	}


	void InputManager::ClearPlayerInputBindings()
	{
		m_pControllerBinds.clear();
		m_pPlayerKeyboardBinds.clear();
	}

	bool InputManager::IsMouseButtonDown() const
	{
		return m_KeyboardInput->IsMouseButtonDown();
	}

	bool InputManager::IsMouseButtonUp() const
	{
		return m_KeyboardInput->IsMouseButtonUp();
	}

	bool InputManager::IsKeyUp(KeyboardButton key)
	{
		return m_KeyboardInput->IsKeyUp(key);
	}

	bool InputManager::IsControllerButtonUp(ControllerButton button)
	{
		bool isUp = false;

		for (size_t i = 0; i < m_pControllers.size(); i++)
		{
			if (m_pControllers[i]->IsUp(button))
			{
				isUp = true;
			}
		}

		return isUp;
	}


	std::string InputManager::GetPressedKeyString()
	{
		return m_KeyboardInput->GetPressedKeyString();
	}

	bool InputManager::IsGameQuit() const
	{
		return m_KeyboardInput->GetIsGameQuit();
	}

	glm::ivec2 InputManager::GetMouseLocation() const
	{
		return m_KeyboardInput->GetMouseLocation();
	}

	void InputManager::ClearFrameEvents() 
	{
		m_KeyboardInput->ClearFrameEvents();
	}
}
