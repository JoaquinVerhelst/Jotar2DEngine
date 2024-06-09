#pragma once
#include "Component.h"
#include "Observer.h"
#include "AIEvents.h"
#include "CollisionEvents.h"

namespace Jotar
{
	class GameObject;


	class ExitComponent final : public Component, public Observer<AIDeathEvent>, public Observer<CollisionEvent>
	{
	public:
		ExitComponent(GameObject* owner, std::string exitTextureFilePath);
		~ExitComponent() = default;

		void SetAmountOfEnemies(int totalEnemies);
		
		void OnNotify(const AIDeathEvent& aiDeathEvent) override;
		void OnNotify(const CollisionEvent& triggerEvent) override;
		void RevealExit();

		ExitComponent(const ExitComponent& other) = delete;
		ExitComponent(ExitComponent&& other) = delete;
		ExitComponent& operator=(const ExitComponent& other) = delete;
		ExitComponent& operator=(ExitComponent&& other) = delete;


	private:
		int m_EnemiesRemaining;
		std::string m_ExitTextureFilePath;
		bool m_IsExitRevealed;
		bool m_ShouldLoadNextLevel;
	};
}
