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

	bool InputManager::ProcessInput()
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

	void InputManager::ControllersHandler()
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

	void InputManager::KeyboardHandler()
	{
		for (const auto& keyboardBinds : m_pKeyboardBinds)
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

	void InputManager::AddKeyBinding(KeyboardKey key, std::unique_ptr<Command> pCommand)
	{
		m_pKeyboardBinds.emplace_back(std::pair(key, std::move(pCommand)));
	}

	bool InputManager::IsMouseButtonDown()
	{
		return m_KeyboardInput->IsMouseButtonDown();
	}

	bool InputManager::IsMouseButtonUp()
	{
		return m_KeyboardInput->IsMouseButtonUp();
	}

	bool InputManager::IsGameQuit()
	{
		return m_KeyboardInput->GetIsGameQuit();
	}

	glm::ivec2 InputManager::GetMouseLocation()
	{
		return m_KeyboardInput->GetMouseLocation();
	}

	void InputManager::ClearFrameEvents()
	{
		m_KeyboardInput->ClearFrameEvents();
	}
}
