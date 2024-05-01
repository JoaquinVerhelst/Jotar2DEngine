#pragma once
#include "Component.h"
#include <vector>
#include "AIFiniteStateMachine.h"
#include <memory>

namespace Jotar
{
	//class MovementComponent;


	//class AIBehaviorComponent final : public Component
	//{
	//public:

	//	explicit AIBehaviorComponent(GameObject* owner);
	//	~AIBehaviorComponent() = default;

	//	void Update() override;

	//	AIBehaviorComponent(const AIBehaviorComponent& other) = delete;
	//	AIBehaviorComponent(AIBehaviorComponent&& other) = delete;
	//	AIBehaviorComponent& operator=(const AIBehaviorComponent& other) = delete;
	//	AIBehaviorComponent& operator=(AIBehaviorComponent&& other) = delete;

	//	void SetStateMachine(std::unique_ptr<AIFiniteStateMachine> stateMachine);

	//private:
	//	MovementComponent* m_pMoveComp;
	//	std::unique_ptr<AIFiniteStateMachine> m_StateMachine;
	//};
}