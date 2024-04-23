#pragma once
#include "Event.h"

namespace Jotar
{
	class EventAddScore : public Event
	{
	public:
		EventAddScore(int addScore);
		virtual ~EventAddScore() = default;

		int GetScoreAdded() const;

	private:
		int m_ScoreAdded;
	};
}

