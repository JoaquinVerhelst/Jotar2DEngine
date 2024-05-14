#pragma once
#include "Component.h"
#include "Subject.h"
#include "HealthEvents.h"
#include <memory>

namespace Jotar
{
	class GameObject;

	class HealthComponent final : public Component   /*, public Subject<EventPlayerDamage>*/
	{
	public:

		HealthComponent(GameObject* owner, int health = 3);
		~HealthComponent();

		virtual void Start() override;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;


		int GetHealth() const;
		void TakeDamage(int damage = 1, GameObject* attacker = nullptr);
		void AddHealth(int health);


		void AddObserver(Observer<HealthEvent>* pObserver)
		{
			m_pSubject->AddObserver(pObserver);
		}
		void RemoveObserver(Observer<HealthEvent>* pObserver)
		{
			m_pSubject->RemoveObserver(pObserver);
		}


	private:

		std::unique_ptr<Subject<HealthEvent>> m_pSubject;
		int m_CurrentHealth;
	};

}