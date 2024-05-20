#pragma once
#include "Component.h"
#include "Observer.h"
#include "Subject.h"
#include "HealthEvents.h"

namespace Jotar
{
	class GameObject;

	class PlayerDeathComponent final : public Component, public Observer<HealthEvent>
	{
	public:

		PlayerDeathComponent(GameObject* owner, float deathPauseTime);
		~PlayerDeathComponent() = default;

		void Update() override;
		void Reset() override;

		void OnNotify(const HealthEvent& eventData) override;

		PlayerDeathComponent(const PlayerDeathComponent& other) = delete;
		PlayerDeathComponent(PlayerDeathComponent&& other) = delete;
		PlayerDeathComponent& operator=(const PlayerDeathComponent& other) = delete;
		PlayerDeathComponent& operator=(PlayerDeathComponent&& other) = delete;

		void AddObserver(Observer<OnDeathEvent>* pObserver)
		{
			m_pSubject->AddObserver(pObserver);
		}
		void RemoveObserver(Observer<OnDeathEvent>* pObserver)
		{
			m_pSubject->RemoveObserver(pObserver);
		}


	private:

		std::unique_ptr<Subject<OnDeathEvent>> m_pSubject;
		bool m_IsDeath;
		float m_DeathTimer;
		float m_DeathPauseTime;
	};

}