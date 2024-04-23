#include "DamageComponent.h"
#include "GameObject.h"

#include "HealthComponent.h"

#include <iostream>

Jotar::DamageComponent::DamageComponent(GameObject* owner, int amountOfDamage)
	:Component(owner)
    , m_Damage{amountOfDamage}
    , m_pDamagedColliders{}
{
}

void Jotar::DamageComponent::OnNotify(const CollisionEvent& triggerEvent)
{
    auto* otherCollider = triggerEvent.GetOtherCollider();

    if (IsColliderAlreadyHit(otherCollider)) return;

    if (otherCollider->GetOwner()->HasComponent<HealthComponent>()) {

        m_pDamagedColliders.emplace_back(triggerEvent.GetOtherCollider());
        triggerEvent.GetOtherCollider()->GetOwner()->GetComponent<HealthComponent>()->TakeDamage(m_Damage);
    }
}

bool Jotar::DamageComponent::IsColliderAlreadyHit(ColliderComponent* otherCollider)
{
    for (size_t i = 0; i < m_pDamagedColliders.size(); i++)
    {
        if (m_pDamagedColliders[i] == otherCollider)
        {
            return true;
        }
    }

    return false;
}
