#pragma once
#include "Event.h"


namespace Jotar
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent() {};

		virtual ~WindowResizeEvent() = default;


	};
}