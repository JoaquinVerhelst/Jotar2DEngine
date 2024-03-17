#pragma once
#include <memory>



namespace Jotar
{
	enum class ControllerButton;

	class ControllerInput final
	{
		class ControllerInputImpl;
		ControllerInputImpl* pImpl;

	public:
		explicit ControllerInput(unsigned int controllerID);
		~ControllerInput();


		void Update();

		bool IsPressed(ControllerButton button) const;
		bool IsDown(ControllerButton button) const;
		bool IsUp(ControllerButton button) const;

		unsigned int GetMaxControllers() const;
	};
}