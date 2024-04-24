#pragma once
#include <map>
#include <vector>
#include <memory>


namespace Jotar
{
	class Blackboard;

	class AIState
	{
	public:
		AIState() {}
		virtual ~AIState() = default;

		virtual void OnEnter(Blackboard* pBlackboard) {};
		virtual void OnExit(Blackboard* pBlackboard) {};
		virtual void Update(Blackboard* pBlackboard) {};

	};

	class AICondition
	{
	public:
		AICondition() = default;
		virtual ~AICondition() = default;
		virtual bool Evaluate(Blackboard* pBlackboard) = 0;
	};


	class AIFiniteStateMachine
	{
		AIFiniteStateMachine(std::shared_ptr<AIState> startState, std::unique_ptr<Blackboard> pBlackboard);
		virtual ~AIFiniteStateMachine() = default;

		void AddTransition(std::shared_ptr<AIState> startState, std::shared_ptr<AIState> toState, std::shared_ptr<AICondition> transition);
		virtual void Update();
		Blackboard* GetBlackboard() const;

	private:
		void ChangeState(std::shared_ptr<AIState> newState);
	private:
		typedef std::pair<std::shared_ptr<AICondition>, std::shared_ptr<AIState>> TransitionStatePair;
		typedef std::vector<TransitionStatePair> Transitions;

		std::map<std::shared_ptr<AIState>, Transitions> m_Transitions; //Key is the state, value are all the transitions for that current state 
		std::shared_ptr<AIState> m_pCurrentState;
		std::unique_ptr<Blackboard> m_pBlackboard = nullptr; // takes ownership of the blackboard

	};
}