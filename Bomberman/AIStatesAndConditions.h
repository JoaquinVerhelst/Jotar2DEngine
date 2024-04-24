#pragma once
#include "Blackboard.h"
#include "AIFiniteStateMachine.h"
#include "glm/glm.hpp"

namespace Jotar
{
	class GoToTargetState : public AIState
	{
	public:
		GoToTargetState() = default;
		~GoToTargetState() = default;
		virtual void OnEnter(Blackboard* pBlackboard) override;
		virtual void Update(Blackboard* pBlackboard) override;
	private:
		std::vector<glm::vec2> m_Path{};
		glm::vec2 m_CurrentDirection{};
	};

	class IdleState : public AIState
	{
	public:
		IdleState() = default;
		~IdleState() = default;
		virtual void OnEnter(Blackboard* pBlackboard) override;
		virtual void Update(Blackboard* pBlackboard) override;
	private:
	};

	class CalculateRandomPathState : public AIState
	{
	public:
		CalculateRandomPathState() = default;
		~CalculateRandomPathState() = default;
		virtual void OnEnter(Blackboard* pBlackboard) override;
		virtual void Update(Blackboard* pBlackboard) override;
	private:

	};


	//--------------------------------------------
	//---TRANSITIONS------------------------------
	//--------------------------------------------

	class HasFinishedThePathCondition : public AICondition
	{
	public:
		HasFinishedThePathCondition() = default;
		virtual bool Evaluate(Blackboard* pBlackboard) override;
	};




}
