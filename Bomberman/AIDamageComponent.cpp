#include "AIDamageComponent.h"

Jotar::AIDamageComponent::AIDamageComponent(GameObject* owner, int amountOfDamage, std::vector<std::string> tagsToHit, GameObject* attacker)
	: DamageComponent(owner, amountOfDamage, tagsToHit, attacker)
{
	m_pSubject = std::make_unique<Subject<AIEvents>>();
}

void Jotar::AIDamageComponent::OnNotify(const CollisionEvent& collisionEvent)
{
	DamageComponent::OnNotify(collisionEvent);

	m_pSubject->NotifyObservers(AIKilledTarget());
}
