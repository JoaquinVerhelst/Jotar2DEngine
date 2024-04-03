#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include <string>

Jotar::HealthDisplayComponent::HealthDisplayComponent(GameObject* owner)
	:Component(owner)
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}


void Jotar::HealthDisplayComponent::OnNotify(EventPlayerDamage* event)
{
    if (event->GetHealth() > 0)
    {
        m_pTextComponent->SetText("Lives: " + std::to_string(event->GetHealth()));
    }
    else
    {
        // Do nothing for now
        m_pTextComponent->SetText("Player Died!");
    }

}
