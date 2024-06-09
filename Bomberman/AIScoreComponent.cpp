#include "AIScoreComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"


Jotar::AIScoreComponent::AIScoreComponent(GameObject* owner, int enemyScore)
	:Component(owner)
	, m_ScoreToAdd{enemyScore}
{}

void Jotar::AIScoreComponent::OnNotify(const AIDeathEvent& aiDeathEvent)
{
	if (aiDeathEvent.GetAttacker() != nullptr)
		aiDeathEvent.GetAttacker()->GetComponent<ScoreComponent>()->AddScore(m_ScoreToAdd);
}
