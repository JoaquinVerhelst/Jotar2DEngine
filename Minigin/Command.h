#pragma once

namespace Jotar
{
	class Command
	{
	public:
		virtual void Execute() = 0;
		virtual ~Command() {}
	};
}