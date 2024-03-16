#pragma once
#include "Singleton.h"

namespace Jotar
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};

}
