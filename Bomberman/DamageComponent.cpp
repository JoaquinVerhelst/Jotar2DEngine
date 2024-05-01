#include "DamageComponent.h"
#include "GameObject.h"

#include "HealthComponent.h"
#include "BombComponent.h"
#include <iostream>

Jotar::DamageComponent::DamageComponent(GameObject* owner, int amountOfDamage)
	:Component(owner)
    , m_Damage{amountOfDamage}
{
}

void Jotar::DamageComponent::OnNotify(const CollisionEvent& triggerEvent)
{ 
    if (typeid(triggerEvent) == typeid(TriggerBeginEvent))
    {
        if (triggerEvent.GetOtherCollider() == nullptr) return;
        auto* otherCollider = triggerEvent.GetOtherCollider();

        if (otherCollider->CompareTag("Killable") || otherCollider->CompareTag("Player"))
        {
            triggerEvent.GetOtherCollider()->GetOwner()->GetComponent<HealthComponent>()->TakeDamage(m_Damage);

            std::cout << "Takign Damagae" << '\n';
        }
    }

    //else if (otherCollider->CompareTag("Bomb"))
    //{
    //    triggerEvent.GetOtherCollider()->GetOwner()->GetComponent<BombComponent>()->Explode();
    //}

}
