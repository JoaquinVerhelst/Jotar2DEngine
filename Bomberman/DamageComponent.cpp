#include "DamageComponent.h"
#include "GameObject.h"

#include "HealthComponent.h"
#include "BombComponent.h"
#include <iostream>

Jotar::DamageComponent::DamageComponent(GameObject* owner, int amountOfDamage, std::vector<std::string> tagsToHit, GameObject* attacker)
	:Component(owner)
    , m_Damage{amountOfDamage}
    , m_TagsToHit{ tagsToHit }
    , m_Attacker{ attacker }
{
}

void Jotar::DamageComponent::OnNotify(const CollisionEvent& triggerEvent)
{ 
    if (typeid(triggerEvent) == typeid(TriggerBeginEvent))
    {
        if (triggerEvent.GetOtherCollider() == nullptr) return;
        auto* otherCollider = triggerEvent.GetOtherCollider();

        for (size_t i = 0; i < m_TagsToHit.size(); i++)
        {
            if (otherCollider->CompareTag(m_TagsToHit[i]))
            {
                triggerEvent.GetOtherCollider()->GetOwner()->GetComponent<HealthComponent>()->TakeDamage(m_Damage, m_Attacker);
            }
        }
    }
}
