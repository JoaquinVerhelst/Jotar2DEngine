#include "AIBehaviorComponent.h"
#include "MovementComponent.h"
#include "GameObject.h"

Jotar::AIBehaviorComponent::AIBehaviorComponent(GameObject* owner)
	:Component(owner)
{
	m_pMoveComp = GetOwner()->GetComponent<MovementComponent>();
}

void Jotar::AIBehaviorComponent::Update()
{
	m_StateMachine->Update();
}

void Jotar::AIBehaviorComponent::SetStateMachine(std::unique_ptr<AIFiniteStateMachine> stateMachine)
{
	m_StateMachine = std::move(stateMachine);
}

