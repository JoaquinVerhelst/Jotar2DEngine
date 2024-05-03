#include "AIStatesAndConditions.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include "WorldGrid.h"
//
//void Jotar::GoToTargetState::OnEnter(Blackboard* pBlackboard)
//{
//
//	if (pBlackboard->GetData("Path", m_Path) == false)
//		return;
//}
//
//void Jotar::GoToTargetState::Update(Blackboard* pBlackboard)
//{
//	MovementComponent* moveComp;
//	if (pBlackboard->GetData("MoveComp", moveComp) == false)
//		return;
//
//	if (m_CurrentDirection == glm::vec2{0, 0})
//	{
//		 Go to the next point of the path
//		glm::vec2 dir = m_Path[0] - moveComp->GetOwner()->GetTransform()->GetWorldPosition();
//
//		if (dir.x == 0 && dir.y <= 0) // Up
//			m_CurrentDirection = { 0, 1 };
//		else if (dir.x == 0 && dir.y >= 0) // Down
//			m_CurrentDirection = { 0, -1 };
//		else if (dir.x <= 0 && dir.y == 0) // Left
//			m_CurrentDirection = { -1, 0 };
//		else if (dir.x >= 0 && dir.y == 0) // Right
//			m_CurrentDirection = { 1, 0 };
//	}
//
//	moveComp->Move(m_CurrentDirection);
//
//	 When reached, remove point from path, and go to the next point
//	if (glm::distance(moveComp->GetTransform()->GetWorldPosition(), m_Path[0]) < 1)
//	{
//		m_Path.erase(m_Path.begin());
//		m_CurrentDirection = glm::vec2{ 0,0 };
//	}
//	 if its the path end -> Go to Idle
//	if (m_Path.empty())
//	{
//		pBlackboard->ChangeData("HasFinishedThePath", true);
//	}
//}
//
//
//
//
//void Jotar::IdleState::OnEnter(Blackboard* pBlackboard)
//{
//}
//
//void Jotar::IdleState::Update(Blackboard* pBlackboard)
//{
//}
//
//
//
//void Jotar::CalculateRandomPathState::OnEnter(Blackboard* pBlackboard)
//{
//	glm::ivec2 randomTarget;
//}
//
//void Jotar::CalculateRandomPathState::Update(Blackboard* pBlackboard)
//{
//}
//
//bool Jotar::HasFinishedThePathCondition::Evaluate(Blackboard* pBlackboard)
//{
//	bool hasFinished = false;
//
//	if (pBlackboard->GetData("HasFinishedThePath", hasFinished) == false)
//		return false;
//
//	if (!hasFinished)
//		return false;
//
//	return true;
//}
