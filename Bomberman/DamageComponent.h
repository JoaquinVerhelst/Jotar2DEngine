#pragma once
#include "Component.h"
#include "Observer.h"
#include "CollisionEvents.h"
#include "ColliderComponent.h"
#include <vector>

namespace Jotar
{
	class DamageComponent : public Component, public Observer<CollisionEvent>
	{
	public:
		DamageComponent(GameObject* owner, int amountOfDamage, std::vector<std::string> tagsToHit);

		void OnNotify(const CollisionEvent& event) override;

		DamageComponent(const DamageComponent& other) = delete;
		DamageComponent(DamageComponent&& other) = delete;
		DamageComponent& operator=(const DamageComponent& other) = delete;
		DamageComponent& operator=(DamageComponent&& other) = delete;

	private:

		int m_Damage;
		std::vector<std::string> m_TagsToHit;
	};

}

