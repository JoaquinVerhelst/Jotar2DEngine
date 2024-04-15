#include "ScoreDisplayComponent.h"
#include "GameObject.h"
#include "SteamAchievements.h"


Jotar::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner)
	:Component(owner)
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}

void Jotar::ScoreDisplayComponent::OnNotify(const EventAddScore& event)
{
    m_pTextComponent->SetText("Score: " + std::to_string(event.GetScoreAdded()));
}
