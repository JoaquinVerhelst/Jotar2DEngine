
#include "EventAddScore.h"

Jotar::EventAddScore::EventAddScore(int addScore)
    : m_ScoreAdded{ addScore }
{
}

int Jotar::EventAddScore::GetScoreAdded() const
{
    return m_ScoreAdded;
}
