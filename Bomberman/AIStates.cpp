#include "AIStates.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include <iostream>

#include "WorldGrid.h"
#include <random>
#include "AIBehaviorComponent.h"

#include "SceneManager.h"
#include "Camera.h"
#include "Scene.h"

Jotar::IdleAIState::IdleAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
{
}

void Jotar::IdleAIState::OnEnter()
{
	// Stand still for 0 - 2 sec
	std::cout << "Idle On eneter" << '\n';
}

Jotar::AIState* Jotar::IdleAIState::OnHandle()
{
	std::cout << "Idle On Handle" << '\n';
	//if waiting time is over -> Calculate Path

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
	std::cout << "GOTO TARGEt On eneter" << '\n';



}

Jotar::AIState* Jotar::GoToTargetAIState::OnHandle()
{
	//std::cout << "GOTO TARGEt On handle " << '\n';


	auto& pos = m_pMovementComp->GetTransform()->GetWorldPosition();


	if (m_Path.empty())
	{
		return m_pAIBehaviorComp->GetIdleState();
	}


	glm::vec2 pathPos = m_Path[0];

	//todo get rid of hardcocded index
	auto camObj = SceneManager::GetInstance().GetScene(0).GetCamera();
	if (camObj != nullptr)
		pathPos += camObj->GetOffset();


	//Go to the next point of the path
	glm::vec2 dir = pathPos - pos;

	if (dir.x < 0) // Moving left
		m_CurrentDirection = { -1, 0 };
	else if (dir.x > 0) // Moving right
		m_CurrentDirection = { 1, 0 };
	else if (dir.y < 0) // Moving up
		m_CurrentDirection = { 0, -1 };
	else if (dir.y > 0) // Moving down
		m_CurrentDirection = { 0, 1 };
	

	m_pMovementComp->Move(m_CurrentDirection);

	// When reached, remove point from path, and go to the next point
	auto distance = glm::distance(pos, pathPos);


	if (distance * distance < 5)
	{
		m_Path.erase(m_Path.begin());
		m_CurrentDirection = glm::vec2{ 0,0 };
	}

	//if its the path end -> Go to Idle


	return nullptr;
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





Jotar::CalculateTargetAIState::CalculateTargetAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
	, m_pNavigationSystem{ std::make_unique<NavigationSystem>()}
	, m_IsPathFound{ false }
{
}

void Jotar::CalculateTargetAIState::OnEnter()
{
	std::cout << "Calculate PAth Enter" << '\n';

	auto& pos = m_pAIBehaviorComp->GetOwner()->GetTransform()->GetLocalPosition();
	// Get Start cell (our position)
	auto startCell = WorldGrid::GetInstance().GetGridCellByPosition(pos);


	//if intelligent && player is seen -> Calculate path based on player position
	// TODO Get a way to check if the player is seen
	bool playerIsSeen = true;
	// Get the end cell -> Random or players current location
	if (!playerIsSeen)
	{
		while (!m_IsPathFound)
		{
			//TODO Get rid of hardCoded range
			auto randomCellIndex = GetNextRandomCellIndex(startCell.Index, 8);

			const auto& endCell = WorldGrid::GetInstance().GetGridCellByID(randomCellIndex);

			if (!endCell.ObjectOnCell.expired()) continue;

			auto path = m_pNavigationSystem->FindPath(startCell, endCell);

			// Sometimes I had a problem that the path had no commecting positions
			int minPathLength = CalculateMinPathLength(startCell.Index, endCell.Index);


			if (!path.empty() && path.size() >= minPathLength)
			{
				m_pAIBehaviorComp->GetGoToTargetState()->SetPath(path);
				m_IsPathFound = true;
			}
		}
	}
	else
	{

	}
}

Jotar::AIState* Jotar::CalculateTargetAIState::OnHandle()
{
	std::cout << "Calculate PAth On Handle " << '\n';

	if (m_IsPathFound)
	{
		m_IsPathFound = false;
		return m_pAIBehaviorComp->GetGoToTargetState();
	}
	else
	{
		return m_pAIBehaviorComp->GetIdleState();
	}
}

int Jotar::CalculateTargetAIState::CalculateMinPathLength(glm::ivec2 startIndex, glm::ivec2 endIndex)
{
	int distanceX = std::abs(endIndex.x - startIndex.x);
	int distanceY = std::abs(endIndex.y - startIndex.y);
	int minPathLength = distanceX + distanceY;

	// Each position is the middle point of a cell, so the minimum path length should be at least the Manhattan distance + 1
	return minPathLength;
}

glm::ivec2 Jotar::CalculateTargetAIState::GetNextRandomCellIndex(glm::ivec2 cellIndex, int wanderRange)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distX(cellIndex.x - wanderRange, cellIndex.x + wanderRange);
	std::uniform_int_distribution<int> distY(cellIndex.y - wanderRange, cellIndex.y + wanderRange);

	glm::ivec2 randomCellIndex;
	randomCellIndex.x = distX(gen);
	randomCellIndex.y = distY(gen);

	auto gridSize = WorldGrid::GetInstance().GetGridSize();

	if (randomCellIndex.x < 0) randomCellIndex.x = 0;
	if (randomCellIndex.x >= gridSize.x) randomCellIndex.x = gridSize.x - 1;
	if (randomCellIndex.y < 0) randomCellIndex.y = 0;
	if (randomCellIndex.y >= gridSize.y) randomCellIndex.y = gridSize.y -1;

	return randomCellIndex;
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
