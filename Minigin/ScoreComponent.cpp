#include "ScoreComponent.h"

Jotar::ScoreComponent::ScoreComponent(GameObject* owner)
	: Component(owner)
	, m_CurrentScore{0}
{
}

Jotar::ScoreComponent::~ScoreComponent()
{
	RemoveAllObservers();
}

void Jotar::ScoreComponent::Start()
{
	NotifyObservers(EventAddScore{ m_CurrentScore });
}

int Jotar::ScoreComponent::GetScore()
{
	return m_CurrentScore;
}

void Jotar::ScoreComponent::AddScore(int score)
{
	m_CurrentScore += score;

	NotifyObservers(EventAddScore{ m_CurrentScore});
}
