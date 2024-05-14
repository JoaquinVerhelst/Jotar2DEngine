#pragma once
#include "Component.h"
#include "Observer.h"
#include "AIEvents.h"

namespace Jotar
{
	class GameObject;


	class ExitComponent : public Component, Observer<AIDeathEvent>
	{
	public:
		ExitComponent(GameObject* owner, int amountOfEnemies);
		~ExitComponent() = default;



		void OnNotify(const AIDeathEvent& aiDeathEvent) override;

		ExitComponent(const ExitComponent& other) = delete;
		ExitComponent(ExitComponent&& other) = delete;
		ExitComponent& operator=(const ExitComponent& other) = delete;
		ExitComponent& operator=(ExitComponent&& other) = delete;


	private:
		int m_EnemiesRemaining;

	};
}
