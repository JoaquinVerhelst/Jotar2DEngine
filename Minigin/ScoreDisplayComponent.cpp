#include "ScoreDisplayComponent.h"
#include "GameObject.h"
#include "SteamAchievements.h"


Jotar::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner)
	:Component(owner)
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}

void Jotar::ScoreDisplayComponent::OnNotify(EventAddScore* event)
{
    m_pTextComponent->SetText("Score: " + std::to_string(event->GetScoreAdded()));


	// todo: Check of je de achievement hebt
	if (event->GetScoreAdded() >= 500)
		CSteamAchievements::GetInstance().SetAchievement("ACH_WIN_ONE_GAME");
}
