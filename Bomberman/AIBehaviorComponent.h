#pragma once
#include "Component.h"
#include <vector>
#include "AIStates.h"
#include <memory>

namespace Jotar
{
	class MovementComponent;


	class AIBehaviorComponent final : public Component
	{
	public:

		explicit AIBehaviorComponent(GameObject* owner);
		~AIBehaviorComponent() = default;

		void Update() override;
		void Render() const override;

		AIBehaviorComponent(const AIBehaviorComponent& other) = delete;
		AIBehaviorComponent(AIBehaviorComponent&& other) = delete;
		AIBehaviorComponent& operator=(const AIBehaviorComponent& other) = delete;
		AIBehaviorComponent& operator=(AIBehaviorComponent&& other) = delete;

		bool GetIsPlayerSeen() const;

		IdleAIState* GetIdleState() const { return m_IdleState.get(); }
		GoToTargetAIState* GetGoToTargetState() const { return m_GoToTargetState.get(); }
		DeadAIState* GetDeadState() const { return m_DeadState.get(); }
		CalculateNextTargetAIState* GetCalculatePathState() const { return m_CalculatePathState.get(); }
		ChaseTargetAIState* GetChaseTargetState() const { return m_ChaseTargetState.get(); }

	private:
		AIState* m_pCurrentState;
		std::unique_ptr<IdleAIState> m_IdleState;
		std::unique_ptr<GoToTargetAIState> m_GoToTargetState;
		std::unique_ptr<DeadAIState> m_DeadState;
		std::unique_ptr<CalculateNextTargetAIState> m_CalculatePathState;
		std::unique_ptr<ChaseTargetAIState> m_ChaseTargetState;

		bool m_IsPlayerSeen;


	};
}