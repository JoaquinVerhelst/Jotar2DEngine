#pragma once


#include "DamageComponent.h"
#include "Observer.h"
#include "CollisionEvents.h"
#include "ColliderComponent.h"
#include "AIEvents.h"
#include "Subject.h"
#include <vector>

namespace Jotar
{
	class AIDamageComponent final : public DamageComponent
	{
	public:
		AIDamageComponent(GameObject* owner, int amountOfDamage, std::vector<std::string> tagsToHit, GameObject* attacker = nullptr);

		virtual void OnNotify(const CollisionEvent& collisionEvent) override;

		AIDamageComponent(const DamageComponent& other) = delete;
		AIDamageComponent(DamageComponent&& other) = delete;
		AIDamageComponent& operator=(const DamageComponent& other) = delete;
		AIDamageComponent& operator=(DamageComponent&& other) = delete;

		void AddObserver(Observer<AIEvents>* pObserver)
		{
			m_pSubject->AddObserver(pObserver);
		}
		void RemoveObserver(Observer<AIEvents>* pObserver)
		{
			m_pSubject->RemoveObserver(pObserver);
		}


	protected:

		std::unique_ptr<Subject<AIEvents>> m_pSubject;
	};

}
