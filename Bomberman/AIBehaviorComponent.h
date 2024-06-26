
#pragma once
#include "Component.h"
#include <vector>
#include "AIStates.h"
#include <memory>
#include "Observer.h"
#include "AIEvents.h"
#include "Subject.h"
#include "Event.h"


namespace Jotar
{
	class MovementComponent;
	class AIAnimationControllerComponent;
	class AIPerceptionComponent;

	class AIBehaviorComponent final : public Component , public Observer<AIEvents>, public Observer<Event>
	{
	public:

		explicit AIBehaviorComponent(GameObject* owner, float chaseRecheckPathTime = 2.0f, int walkRange = 3, float deathWaitTime = 2.f);
		~AIBehaviorComponent();

		void Start() override;
		void Update() override;


		AIBehaviorComponent(const AIBehaviorComponent& other) = delete;
		AIBehaviorComponent(AIBehaviorComponent&& other) = delete;
		AIBehaviorComponent& operator=(const AIBehaviorComponent& other) = delete;
		AIBehaviorComponent& operator=(AIBehaviorComponent&& other) = delete;

		bool GetIsPlayerSeen() const;
		void OnNotify(const AIEvents& event) override;
		void OnNotify(const Event& event) override;

		GoToTargetAIState* GetGoToTargetState() const { return m_pGoToTargetState.get(); }
		CalculateRandomPathAIState* GetCalculateRandomPathState() const { return m_pCalculateRandomPathState.get(); }
		ChaseTargetAIState* GetChaseTargetState() const { return m_pChaseTargetState.get(); }
		CalculatePathToPlayerAIState* GetCalculatePathToPlayerState() const { return m_pCalculatePathToPlayerState.get(); }
		OnDamageAIState* GetOnDamageState() { return m_pOnDamageState.get(); }

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
		std::unique_ptr<GoToTargetAIState> m_pGoToTargetState;
		std::unique_ptr<CalculateRandomPathAIState> m_pCalculateRandomPathState;
		std::unique_ptr<CalculatePathToPlayerAIState> m_pCalculatePathToPlayerState;
		std::unique_ptr<ChaseTargetAIState> m_pChaseTargetState;
		std::unique_ptr<OnDamageAIState> m_pOnDamageState;


		AIAnimationControllerComponent* m_pAIAnimationControllerComponent;
		AIPerceptionComponent* m_pAIPerceptionComponent;

		bool m_IsPlayerSeen;
		bool m_IsDamaged;


		std::unique_ptr<Subject<AIDeathEvent>> m_pSubject;
	};
}