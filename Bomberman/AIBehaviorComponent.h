#pragma once
#include "Component.h"
#include <vector>
#include "AIStates.h"
#include <memory>
#include "Observer.h"
#include "AIEvents.h"
#include "HealthEvents.h"


namespace Jotar
{
	class MovementComponent;
	class AIAnimationControllerComponent;






	class AIBehaviorComponent final : public Component , public Observer<AIPlayerSeenEvent>, public Observer<HealthEvent>
	{
	public:

		explicit AIBehaviorComponent(GameObject* owner);
		~AIBehaviorComponent() = default;

		void Start() override;
		void Update() override;
		void Render() const override;

		AIBehaviorComponent(const AIBehaviorComponent& other) = delete;
		AIBehaviorComponent(AIBehaviorComponent&& other) = delete;
		AIBehaviorComponent& operator=(const AIBehaviorComponent& other) = delete;
		AIBehaviorComponent& operator=(AIBehaviorComponent&& other) = delete;

		bool GetIsPlayerSeen() const;
		void OnNotify(const AIPlayerSeenEvent& event) override;
		void OnNotify(const HealthEvent& event) override;

		IdleAIState* GetIdleState() const { return m_IdleState.get(); }
		GoToTargetAIState* GetGoToTargetState() const { return m_GoToTargetState.get(); }
		OnDamageAIState* GetOnDamageAIState() const { return m_OnDamageAIState.get(); }
		CalculateRandomPathAIState* GetCalculateRandomPathState() const { return m_CalculateRandomPathState.get(); }
		ChaseTargetAIState* GetChaseTargetState() const { return m_ChaseTargetState.get(); }
		CalculatePathToPlayerAIState* GetCalculatePathToPlayerState() const { return m_CalculatePathToPlayerState.get(); }

		AIAnimationControllerComponent* GetAnimatorController() { return m_pAIAnimationControllerComponent; }

	private:

		AIState* m_pCurrentState;
		std::unique_ptr<IdleAIState> m_IdleState;
		std::unique_ptr<GoToTargetAIState> m_GoToTargetState;
		std::unique_ptr<OnDamageAIState> m_OnDamageAIState;
		std::unique_ptr<CalculateRandomPathAIState> m_CalculateRandomPathState;
		std::unique_ptr<CalculatePathToPlayerAIState> m_CalculatePathToPlayerState;
		std::unique_ptr<ChaseTargetAIState> m_ChaseTargetState;

		AIAnimationControllerComponent* m_pAIAnimationControllerComponent;

		bool m_IsPlayerSeen;
		bool m_IsDamaged;

	};
}