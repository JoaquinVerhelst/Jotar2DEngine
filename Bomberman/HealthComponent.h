#pragma once
#include "Component.h"
#include "Subject.h"
#include "HealthEvents.h"
#include <memory>

namespace Jotar
{
	class GameObject;

	class HealthComponent : public Component
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
		virtual void TakeDamage(int damage = 1, GameObject* attacker = nullptr);
		void AddHealth(int health);


		void AddObserver(Observer<Event>* pObserver)
		{
			m_pSubject->AddObserver(pObserver);
		}
		void RemoveObserver(Observer<Event>* pObserver)
		{
			m_pSubject->RemoveObserver(pObserver);
		}


	protected:

		std::unique_ptr<Subject<Event>> m_pSubject;
		int m_CurrentHealth;
	};

}