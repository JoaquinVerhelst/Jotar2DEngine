#pragma once
#include "HealthComponent.h"
#include "Observer.h"
#include "Subject.h"
#include "HealthEvents.h"

namespace Jotar
{
	class GameObject;

	class PlayerHealthComponent final : public HealthComponent
	{
	public:

		PlayerHealthComponent(GameObject* owner, int health, float deathPauseTime = 2.f);
		~PlayerHealthComponent() = default;

		void Update() override;
		void Reset() override;
		bool GetIsDeath() { return m_IsDeath; }

		//void OnNotify(const HealthEvent& eventData) override;
		void TakeDamage(int damage = 1, GameObject* attacker = nullptr) override;


		PlayerHealthComponent(const PlayerHealthComponent& other) = delete;
		PlayerHealthComponent(PlayerHealthComponent&& other) = delete;
		PlayerHealthComponent& operator=(const PlayerHealthComponent& other) = delete;
		PlayerHealthComponent& operator=(PlayerHealthComponent&& other) = delete;

		void AddOnDeathObserver(Observer<Event>* pObserver)
		{
			m_pOnDeathSubject->AddObserver(pObserver);
		}
		void RemoveOnDeathObserver(Observer<Event>* pObserver)
		{
			m_pOnDeathSubject->RemoveObserver(pObserver);
		}


	private:

		std::unique_ptr<Subject<Event>> m_pOnDeathSubject;
		bool m_IsDeath;
		float m_DeathTimer;
		float m_DeathPauseTime;
	};

}