#pragma once
#include "HealthComponent.h"
#include "Observer.h"
#include "Subject.h"
#include "AIEvents.h"

namespace Jotar
{
	class GameObject;

	class BalloomPlayerHealthComponent final : public HealthComponent
	{
	public:

		BalloomPlayerHealthComponent(GameObject* owner, int health, float deathPauseTime = 2.f);
		~BalloomPlayerHealthComponent() = default;

		void Update() override;
		void Reset() override;

		//void OnNotify(const HealthEvent& eventData) override;
		void TakeDamage(int damage = 1, GameObject* attacker = nullptr) override;

		BalloomPlayerHealthComponent(const BalloomPlayerHealthComponent& other) = delete;
		BalloomPlayerHealthComponent(BalloomPlayerHealthComponent&& other) = delete;
		BalloomPlayerHealthComponent& operator=(const BalloomPlayerHealthComponent& other) = delete;
		BalloomPlayerHealthComponent& operator=(BalloomPlayerHealthComponent&& other) = delete;


		void AddOnDeathObserver(Observer<AIDeathEvent>* pObserver)
		{
			m_pOnDeathSubject->AddObserver(pObserver);
		}
		void RemoveOnDeathObserver(Observer<AIDeathEvent>* pObserver)
		{
			m_pOnDeathSubject->RemoveObserver(pObserver);
		}


	private:

		std::unique_ptr<Subject<AIDeathEvent>> m_pOnDeathSubject;

		int m_MaxHealth;
		bool m_IsDeath;
		float m_DeathTimer;
		float m_DeathPauseTime;
	};

}