#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include <string>

Jotar::HealthDisplayComponent::HealthDisplayComponent(GameObject* owner)
	:Component(owner)
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}


void Jotar::HealthDisplayComponent::OnNotify(const HealthEvent& event)
{
    if (typeid(event) == typeid(DamageHealthEvent))
    {
        const DamageHealthEvent& damageEvent = static_cast<const DamageHealthEvent&>(event);

        if (damageEvent.GetHealth() > 0)
        {
            m_pTextComponent->SetText("Lives: " + std::to_string(damageEvent.GetHealth()));
        }
        else
        {

            m_pTextComponent->SetText("Player Died!");
        }
    }
    else if (typeid(event) == typeid(UpdateHealthEvent))
    {
        const UpdateHealthEvent& updateHealthEvent = static_cast<const UpdateHealthEvent&>(event);

        m_pTextComponent->SetText("Lives: " + std::to_string(updateHealthEvent.GetHealth()));
    }
}