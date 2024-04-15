#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include <string>

Jotar::HealthDisplayComponent::HealthDisplayComponent(GameObject* owner)
	:Component(owner)
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}


void Jotar::HealthDisplayComponent::OnNotify(const PlayerHealthEvent& event)
{
    if (typeid(event) == typeid(EventPlayerDamage))
    {
        const EventPlayerDamage& damageEvent = static_cast<const EventPlayerDamage&>(event);

        if (damageEvent.GetHealth() > 0)
        {
            m_pTextComponent->SetText("Lives: " + std::to_string(damageEvent.GetHealth()));
        }
        else
        {
            m_pTextComponent->SetText("Player Died!");
        }
    }
    else if (typeid(event) == typeid(EventUpdatePlayerHealth))
    {
        const EventUpdatePlayerHealth& updateHealthEvent = static_cast<const EventUpdatePlayerHealth&>(event);

        m_pTextComponent->SetText("Lives: " + std::to_string(updateHealthEvent.GetHealth()));
    }
}