#pragma once
#include "Component.h"
#include "Observer.h"
#include "AIEvents.h"

namespace Jotar
{
	class GameObject;


	class AIScoreComponent : public Component, public Observer<AIDeathEvent>
	{
	public:
		AIScoreComponent(GameObject* owner, int enemyScore);


		void OnNotify(const AIDeathEvent& aiDeathEvent) override;

		AIScoreComponent(const AIScoreComponent& other) = delete;
		AIScoreComponent(AIScoreComponent&& other) = delete;
		AIScoreComponent& operator=(const AIScoreComponent& other) = delete;
		AIScoreComponent& operator=(AIScoreComponent&& other) = delete;

	private:

		int m_ScoreToAdd;
	};
}
