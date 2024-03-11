#pragma once
#include <memory>

namespace dae
{
	class Command
	{
	public:
		virtual void Execute() = 0;
		virtual ~Command() {}
	};


}