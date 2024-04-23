#pragma once
#include "Event.h"

namespace Jotar
{
	class ExplosionEvent : public Event
	{
	public:
		ExplosionEvent() = default;
		virtual ~ExplosionEvent() = default;
	};
}