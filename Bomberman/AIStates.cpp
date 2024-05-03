#include "AIStates.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include <iostream>

#include "GameManager.h"
#include <random>
#include "AIBehaviorComponent.h"

#include "SceneManager.h"
#include "Camera.h"
#include "Scene.h"
#include "WorldTimeManager.h"

Jotar::IdleAIState::IdleAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
{
}

void Jotar::IdleAIState::OnEnter()
{
}

Jotar::AIState* Jotar::IdleAIState::OnHandle()
{
	return m_pAIBehaviorComp->GetCalculatePathState();
}

void Jotar::IdleAIState::OnExit()
{
}



Jotar::GoToTargetAIState::GoToTargetAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
	, m_pMovementComp{ pAiComp->GetOwner()->GetComponent<MovementComponent>() }
{
}


void Jotar::GoToTargetAIState::OnEnter()
{
}

Jotar::AIState* Jotar::GoToTargetAIState::OnHandle()
{
	//std::cout << "GOTO TARGEt On handle " << '\n';


	auto pos = m_pMovementComp->GetTransform()->GetWorldPosition();

	//if its the path end -> Go to Idle
	if (m_Path.empty())
	{
		return m_pAIBehaviorComp->GetIdleState();
	}

	glm::vec2 pathPos = m_Path[0];

	//todo get rid of hardcocded index
	auto camObj = SceneManager::GetInstance().GetScene(0).GetCamera();
	if (camObj != nullptr)
		pathPos += camObj->GetOffset();

	CalculateDirection(pathPos, pos);
	
	m_pMovementComp->Move(m_CurrentDirection);

	return CheckDistanceToPoint(pathPos, pos);
}



void Jotar::GoToTargetAIState::OnExit()
{
}

void Jotar::GoToTargetAIState::SetPath(std::vector<glm::vec2>& path)
{
	m_Path = path;
}

std::vector<glm::vec2> Jotar::GoToTargetAIState::GetPath() const
{
	return m_Path;
}

void Jotar::GoToTargetAIState::CalculateDirection(glm::vec2& pathPos, glm::vec2& AiPos)
{
	//Go to the next point of the path
	glm::vec2 dir = pathPos - AiPos;

	if (dir.x < 0) // Moving left
		m_CurrentDirection = { -1, 0 };
	else if (dir.x > 0) // Moving right
		m_CurrentDirection = { 1, 0 };
	else if (dir.y < 0) // Moving up
		m_CurrentDirection = { 0, -1 };
	else if (dir.y > 0) // Moving down
		m_CurrentDirection = { 0, 1 };
}

Jotar::AIState* Jotar::GoToTargetAIState::CheckDistanceToPoint(glm::vec2 pathPos, glm::vec2 AiPos)
{
	auto distance = glm::distance(AiPos, pathPos);

	if (distance * distance < 5)
	{
		m_Path.erase(m_Path.begin());
		m_CurrentDirection = glm::vec2{ 0,0 };
	}
	return nullptr;
}




Jotar::ChaseTargetAIState::ChaseTargetAIState(AIBehaviorComponent* pAiComp)
	: GoToTargetAIState(pAiComp)
	, m_TimerCounter{}
	, m_ResetPathTime{2.f}
	, m_ShouldReset{false}
{
}

void Jotar::ChaseTargetAIState::OnEnter()
{
	m_TimerCounter = 0;
	m_ShouldReset = false;
}

Jotar::AIState* Jotar::ChaseTargetAIState::OnHandle()
{
	m_TimerCounter += WorldTimeManager::GetInstance().GetDeltaTime();
	if (m_ResetPathTime <= m_TimerCounter)
	{
		m_ShouldReset = true;
	}

	return GoToTargetAIState::OnHandle();
}

void Jotar::ChaseTargetAIState::OnExit()
{
}

Jotar::AIState* Jotar::ChaseTargetAIState::CheckDistanceToPoint(glm::vec2 pathPos, glm::vec2 AiPos)
{
	auto distance = glm::distance(AiPos, pathPos);

	if (distance * distance < 5)
	{
		m_Path.erase(m_Path.begin());
		m_CurrentDirection = glm::vec2{ 0,0 };

		if (m_ShouldReset)
			return m_pAIBehaviorComp->GetCalculatePathState();
	}

	return nullptr;
}





Jotar::CalculateNextTargetAIState::CalculateNextTargetAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
	, m_pNavigationSystem{ std::make_unique<NavigationSystem>()}
	, m_IsPathFound{ false }
{
}

void Jotar::CalculateNextTargetAIState::OnEnter()
{
}

Jotar::AIState* Jotar::CalculateNextTargetAIState::OnHandle()
{
	auto* worldGrid = GameManager::GetInstance().GetWorldGrid();
	auto& pos = m_pAIBehaviorComp->GetOwner()->GetTransform()->GetLocalPosition();
	// Get Start cell (our position)
	auto startCell = worldGrid->GetGridCellByPosition(pos);

	//if intelligent && player is seen -> Calculate path based on player position
	// TODO Get a way to check if the player is seen
	bool playerIsSeen = m_pAIBehaviorComp->GetIsPlayerSeen();
	// Get the end cell -> Random or players current location
	if (!playerIsSeen)
	{
		while (!m_IsPathFound)
		{
			//TODO Get rid of hardCoded range
			auto randomCellIndex = GetNextRandomCellIndex(startCell.Index, 8);
			const auto& endCell = worldGrid->GetGridCellByID(randomCellIndex);
			if (!endCell.ObjectOnCell.expired()) continue;

			auto path = m_pNavigationSystem->FindPath(startCell, endCell);
			if (!path.empty())
			{
				m_pAIBehaviorComp->GetGoToTargetState()->SetPath(path);
				return m_pAIBehaviorComp->GetGoToTargetState();
			}
		}
	}
	else
	{
		while (!m_IsPathFound)
		{
			const auto& endCell = worldGrid->GetGridCellByPosition(GameManager::GetInstance().GetPlayerOne()->GetTransform()->GetLocalPosition());

			auto path = m_pNavigationSystem->FindPath(startCell, endCell);
			if (!path.empty())
			{
				m_pAIBehaviorComp->GetChaseTargetState()->SetPath(path);
				return m_pAIBehaviorComp->GetChaseTargetState();
			}
		}
	}

	return nullptr;
}



int Jotar::CalculateNextTargetAIState::CalculateMinPathLength(glm::ivec2 startIndex, glm::ivec2 endIndex)
{
	int distanceX = std::abs(endIndex.x - startIndex.x);
	int distanceY = std::abs(endIndex.y - startIndex.y);
	int minPathLength = distanceX + distanceY;

	// Each position is the middle point of a cell, so the minimum path length should be at least the Manhattan distance + 1
	return minPathLength;
}

glm::ivec2 Jotar::CalculateNextTargetAIState::GetNextRandomCellIndex(glm::ivec2 cellIndex, int wanderRange)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distX(cellIndex.x - wanderRange, cellIndex.x + wanderRange);
	std::uniform_int_distribution<int> distY(cellIndex.y - wanderRange, cellIndex.y + wanderRange);

	glm::ivec2 randomCellIndex;
	randomCellIndex.x = distX(gen);
	randomCellIndex.y = distY(gen);

	auto gridSize = GameManager::GetInstance().GetWorldGrid()->GetGridSize();

	if (randomCellIndex.x < 0) randomCellIndex.x = 0;
	if (randomCellIndex.x >= gridSize.x) randomCellIndex.x = gridSize.x - 1;
	if (randomCellIndex.y < 0) randomCellIndex.y = 0;
	if (randomCellIndex.y >= gridSize.y) randomCellIndex.y = gridSize.y -1;

	return randomCellIndex;
}

bool Jotar::CalculateNextTargetAIState::CalculatePath(const GridCell& startCell, const GridCell& endCell)
{
	auto path = m_pNavigationSystem->FindPath(startCell, endCell);
	if (!path.empty())
	{
		m_pAIBehaviorComp->GetGoToTargetState()->SetPath(path);
		return true;
	}

	return false;
}



Jotar::DeadAIState::DeadAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
{
}

void Jotar::DeadAIState::OnEnter()
{
}

Jotar::AIState* Jotar::DeadAIState::OnHandle()
{
	return nullptr;
}

void Jotar::DeadAIState::OnExit()
{
}



