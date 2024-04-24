#include "DamageComponent.h"
#include "GameObject.h"

#include "HealthComponent.h"
#include "BombComponent.h"
#include <iostream>

Jotar::DamageComponent::DamageComponent(GameObject* owner, int amountOfDamage)
	:Component(owner)
    , m_Damage{amountOfDamage}
    , m_pDamagedColliders{}
{
}

void Jotar::DamageComponent::OnNotify(const CollisionEvent& triggerEvent)
{
    if (triggerEvent.GetOtherCollider() == nullptr) return;
    auto* otherCollider = triggerEvent.GetOtherCollider();

    if (IsColliderAlreadyHit(otherCollider)) return;

    if (otherCollider->CompareTag("Killable"))
    {
        triggerEvent.GetOtherCollider()->GetOwner()->GetComponent<HealthComponent>()->TakeDamage(m_Damage);
    }
    //else if (otherCollider->CompareTag("Bomb"))
    //{
    //    triggerEvent.GetOtherCollider()->GetOwner()->GetComponent<BombComponent>()->Explode();
    //}

    m_pDamagedColliders.emplace_back(triggerEvent.GetOtherCollider());
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
