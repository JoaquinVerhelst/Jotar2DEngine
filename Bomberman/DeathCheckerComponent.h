#pragma once
#include "Component.h"
#include "Observer.h"
#include "HealthEvents.h"


namespace Jotar
{
	class GameObject;

	class DeathCheckerComponent final : public Component, public Observer<OnDeathEvent>
	{
	public:

		DeathCheckerComponent(GameObject* owner);
		~DeathCheckerComponent() = default;


		void OnNotify(const OnDeathEvent& eventData) override;

		DeathCheckerComponent(const DeathCheckerComponent& other) = delete;
		DeathCheckerComponent(DeathCheckerComponent&& other) = delete;
		DeathCheckerComponent& operator=(const DeathCheckerComponent& other) = delete;
		DeathCheckerComponent& operator=(DeathCheckerComponent&& other) = delete;


	private:
		int m_AmountOfDeathPlayers;
	};

}