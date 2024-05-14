
#pragma once
#include "Component.h"
#include <vector>
#include "AIStates.h"
#include <memory>
#include "Observer.h"
#include "AIEvents.h"
#include "HealthEvents.h"
#include "Subject.h"
#include "Event.h"


namespace Jotar
{
	class MovementComponent;
	class AIAnimationControllerComponent;






	class AIBehaviorComponent final : public Component , public Observer<AIPlayerSeenEvent>, public Observer<HealthEvent>
	{
	public:

		explicit AIBehaviorComponent(GameObject* owner);
		~AIBehaviorComponent();

		void Start() override;
		void Update() override;


		AIBehaviorComponent(const AIBehaviorComponent& other) = delete;
		AIBehaviorComponent(AIBehaviorComponent&& other) = delete;
		AIBehaviorComponent& operator=(const AIBehaviorComponent& other) = delete;
		AIBehaviorComponent& operator=(AIBehaviorComponent&& other) = delete;

		bool GetIsPlayerSeen() const;
		void OnNotify(const AIPlayerSeenEvent& event) override;
		void OnNotify(const HealthEvent& event) override;

		IdleAIState* GetIdleState() const { return m_pIdleState.get(); }
		GoToTargetAIState* GetGoToTargetState() const { return m_pGoToTargetState.get(); }
		CalculateRandomPathAIState* GetCalculateRandomPathState() const { return m_pCalculateRandomPathState.get(); }
		ChaseTargetAIState* GetChaseTargetState() const { return m_pChaseTargetState.get(); }
		CalculatePathToPlayerAIState* GetCalculatePathToPlayerState() const { return m_pCalculatePathToPlayerState.get(); }

		AIAnimationControllerComponent* GetAnimatorController() { return m_pAIAnimationControllerComponent; }


		Subject<AIDeathEvent>* GetSubject() { return m_pSubject.get(); }

		void AddObserver(Observer<AIDeathEvent>* pObserver)
		{
			m_pSubject->AddObserver(pObserver);
		}
		void RemoveObserver(Observer<AIDeathEvent>* pObserver)
		{
			m_pSubject->RemoveObserver(pObserver);
		}


	private:

		AIState* m_pCurrentState;
		std::unique_ptr<IdleAIState> m_pIdleState;
		std::unique_ptr<GoToTargetAIState> m_pGoToTargetState;
		std::unique_ptr<CalculateRandomPathAIState> m_pCalculateRandomPathState;
		std::unique_ptr<CalculatePathToPlayerAIState> m_pCalculatePathToPlayerState;
		std::unique_ptr<ChaseTargetAIState> m_pChaseTargetState;

		AIAnimationControllerComponent* m_pAIAnimationControllerComponent;

		bool m_IsPlayerSeen;
		bool m_IsDamaged;


		std::unique_ptr<Subject<AIDeathEvent>> m_pSubject;
	};
}