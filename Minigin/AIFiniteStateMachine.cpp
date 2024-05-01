#include "AIFiniteStateMachine.h"
#include "Blackboard.h"


//Jotar::AIFiniteStateMachine::AIFiniteStateMachine(std::shared_ptr<AIState> startState, std::unique_ptr<Blackboard> pBlackboard)
//    : m_pCurrentState(nullptr),
//    m_pBlackboard(std::move(pBlackboard))
//{
//    ChangeState(startState);
//}
//
//void Jotar::AIFiniteStateMachine::AddTransition(std::shared_ptr<AIState> startState, std::shared_ptr<AIState> toState, std::shared_ptr<AICondition> condition)
//{
//    auto it = m_Transitions.find(startState);
//    if (it == m_Transitions.end())
//    {
//        m_Transitions[startState] = Transitions();
//    }
//
//    m_Transitions[startState].push_back(std::make_pair(condition, toState));
//}
//
//void Jotar::AIFiniteStateMachine::Update()
//{
//    auto currentTransitions = m_Transitions.find(m_pCurrentState);
//
//    if (currentTransitions != m_Transitions.end())
//    {
//        for (auto& transition : currentTransitions->second)
//        {
//            auto condition = transition.first;
//            auto nextState = transition.second;
//
//            if (condition->Evaluate(m_pBlackboard.get()))
//                ChangeState(nextState);
//        }
//    }
//
//    if (m_pCurrentState != nullptr)
//        m_pCurrentState->Update(m_pBlackboard.get());
//}
//
//Jotar::Blackboard* Jotar::AIFiniteStateMachine::GetBlackboard() const
//{
//	return m_pBlackboard.get();
//}
//
//void Jotar::AIFiniteStateMachine::ChangeState(std::shared_ptr<AIState> newState)
//{
//    if (m_pCurrentState != nullptr)
//        m_pCurrentState->OnExit(m_pBlackboard.get());
//
//    m_pCurrentState = newState;
//
//
//    if (m_pCurrentState != nullptr)
//        m_pCurrentState->OnEnter(m_pBlackboard.get());
//    else
//        std::cout << "received a nullptr state instead of a valid state!" << std::endl;
//}
