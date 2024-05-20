#include "AIStates.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include <iostream>

#include "GameManager.h"
#include <random>
#include "AIBehaviorComponent.h"

#include "SceneManager.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "WorldTimeManager.h"
#include "AIAnimationControllerComponent.h"

#include "ScoreComponent.h"
#include "HealthComponent.h"
#include "AIEvents.h"


// ------------------------------------------------------------------------------------- //
//									IDLE STATE											 //
// ------------------------------------------------------------------------------------- //

Jotar::IdleAIState::IdleAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
{
}

void Jotar::IdleAIState::OnEnter()
{
}

Jotar::AIState* Jotar::IdleAIState::OnHandle()
{
	return m_pAIBehaviorComp->GetCalculateRandomPathState();
}

void Jotar::IdleAIState::OnExit()
{
}

// ------------------------------------------------------------------------------------- //
//								GO TO TARGET STATE										 //
// ------------------------------------------------------------------------------------- //


Jotar::GoToTargetAIState::GoToTargetAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
	, m_pMovementComp{ pAiComp->GetOwner()->GetComponent<MovementComponent>() }
{
}

Jotar::AIState* Jotar::GoToTargetAIState::OnHandle()
{

	//if its the path end -> Go to Idle
	if (m_Path.empty())
	{
		return m_pAIBehaviorComp->GetIdleState();
	}

	auto pos = m_pMovementComp->GetTransform()->GetWorldPosition();
	glm::vec2 pathPos{};

	MoveTowardsNextPoint(pos, pathPos);

	CheckDistanceToPoint(pathPos, pos);

	return nullptr;
}

void Jotar::GoToTargetAIState::SetPath(std::vector<glm::vec2>& path)
{
	m_Path = path;
}

std::vector<glm::vec2> Jotar::GoToTargetAIState::GetPath() const
{
	return m_Path;
}

glm::ivec2 Jotar::GoToTargetAIState::GetCurrentDirection()
{
	return m_CurrentDirection;
}

void Jotar::GoToTargetAIState::MoveTowardsNextPoint(glm::vec2& pos, glm::vec2& pathPos)
{
	pathPos = m_Path[0];

	auto camObj = SceneManager::GetInstance().GetCurrentScene().GetCamera();
	if (camObj != nullptr)
		pathPos += camObj->GetOffset();

	CalculateDirection(pos, pathPos);

	m_pMovementComp->Move(m_CurrentDirection);
}


void Jotar::GoToTargetAIState::CalculateDirection(glm::vec2& pos, glm::vec2& pathPos)
{
	glm::vec2 dir = pathPos - pos;


	dir = glm::normalize(dir);
	

	m_CurrentDirection = glm::ivec2(std::round(dir.x), std::round(dir.y));

	CheckForAnimationUpdate();
}

bool Jotar::GoToTargetAIState::CheckDistanceToPoint(glm::vec2& pos, glm::vec2& pathPos)
{
	auto distance = glm::distance(pos, pathPos);

	if (distance * distance < 3)
	{
		m_Path.erase(m_Path.begin());
		//m_CurrentDirection = glm::vec2{ 0,0 };
		return true;
	}

	return false;
}

void Jotar::GoToTargetAIState::CheckForAnimationUpdate()
{
	if (m_CurrentDirection != m_PreviousDirection)
	{
		m_pAIBehaviorComp->GetAnimatorController()->SetWalkAnimation(m_CurrentDirection);
		m_PreviousDirection = m_CurrentDirection;
	}
}


// ------------------------------------------------------------------------------------- //
//								CHASE TARGET STATE										 //
// ------------------------------------------------------------------------------------- //

Jotar::ChaseTargetAIState::ChaseTargetAIState(AIBehaviorComponent* pAiComp, float updatePathTime)
	: GoToTargetAIState(pAiComp)
	, m_TimerCounter{}
	, m_ResetPathTime{ updatePathTime }
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

	if (m_Path.empty())
	{
		return m_pAIBehaviorComp->GetCalculatePathToPlayerState();
	}

	auto pos = m_pMovementComp->GetTransform()->GetWorldPosition();
	glm::vec2 pathPos{};

	MoveTowardsNextPoint(pos, pathPos);

	if (CheckDistanceToPoint(pathPos, pos) && m_ShouldReset)
	{
		return m_pAIBehaviorComp->GetCalculatePathToPlayerState();
	}

	return nullptr;
}


// ------------------------------------------------------------------------------------- //
//							CALCULATE RANDOM PATH  STATE								 //
// ------------------------------------------------------------------------------------- //


Jotar::CalculateRandomPathAIState::CalculateRandomPathAIState(AIBehaviorComponent* pAiComp, int walkRange)
	: AIState(pAiComp)
	, m_pNavigationSystem{ std::make_unique<NavigationSystem>()}
	, m_IsPathFound{ false }
	, m_Range{ walkRange }
{
}


Jotar::AIState* Jotar::CalculateRandomPathAIState::OnHandle()
{
	auto* worldGrid = GameManager::GetInstance().GetWorldGrid();
	auto& pos = m_pAIBehaviorComp->GetOwner()->GetTransform()->GetLocalPosition();
	// Get Start cell (our position)
	auto startCell = worldGrid->GetGridCellByPosition(pos);

	while (!m_IsPathFound)
	{
		//TODO Get rid of hardCoded range
		auto randomCellIndex = GetNextRandomCellIndex(startCell.Index, m_Range);
		const auto& endCell = worldGrid->GetGridCellByID(randomCellIndex);
		if (!endCell.ObjectOnCell.expired()) continue;

		auto path = m_pNavigationSystem->FindPath(startCell, endCell);
		if (!path.empty())
		{
			m_pAIBehaviorComp->GetGoToTargetState()->SetPath(path);
			return m_pAIBehaviorComp->GetGoToTargetState();
		}
	}
	
	return nullptr;
}


glm::ivec2 Jotar::CalculateRandomPathAIState::GetNextRandomCellIndex(glm::ivec2 cellIndex, int wanderRange) const
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

bool Jotar::CalculateRandomPathAIState::CalculatePath(const GridCell& startCell, const GridCell& endCell)
{
	auto path = m_pNavigationSystem->FindPath(startCell, endCell);
	if (!path.empty())
	{
		m_pAIBehaviorComp->GetGoToTargetState()->SetPath(path);
		return true;
	}

	return false;
}



// ------------------------------------------------------------------------------------- //
//							CALCULATE PATH TO PLAYER STATE								 //
// ------------------------------------------------------------------------------------- //


Jotar::CalculatePathToPlayerAIState::CalculatePathToPlayerAIState(AIBehaviorComponent* pAiComp)
	:CalculateRandomPathAIState(pAiComp, 0)
	, m_pTargetCollider{ nullptr }
{
}



Jotar::AIState* Jotar::CalculatePathToPlayerAIState::OnHandle()
{
	if (m_pTargetCollider == nullptr)
	{
		std::cout << "Target is null " << '\n';
		return nullptr;
	}

	auto* worldGrid = GameManager::GetInstance().GetWorldGrid();
	auto& pos = m_pAIBehaviorComp->GetOwner()->GetTransform()->GetLocalPosition();
	auto startCell = worldGrid->GetGridCellByPosition(pos);

	while (!m_IsPathFound)
	{
		const auto& endCell = worldGrid->GetGridCellByPosition(m_pTargetCollider->GetTransform()->GetLocalPosition());

		auto path = m_pNavigationSystem->FindPath(startCell, endCell);
		if (!path.empty())
		{
			m_pAIBehaviorComp->GetChaseTargetState()->SetPath(path);
			return m_pAIBehaviorComp->GetChaseTargetState();
		}
	}

	return nullptr;
}

void Jotar::CalculatePathToPlayerAIState::SetTarget(ColliderComponent* targetCollider)
{
	m_pTargetCollider = targetCollider;
}



// ------------------------------------------------------------------------------------- //
//									DEAD STATE											 //
// ------------------------------------------------------------------------------------- //

Jotar::OnDamageAIState::OnDamageAIState(AIBehaviorComponent* pAiComp)
	: AIState(pAiComp)
	, m_DeathWaitTime{ 0 }
	, m_IsDeath{ false }
	, m_DeathTimer{ 0 }
	, m_Attacker{ nullptr }
{
}

void Jotar::OnDamageAIState::OnEnter()
{
	m_pAIBehaviorComp->GetAnimatorController()->SetDamageAnimation();
	m_DeathTimer = 0;
	m_pAIBehaviorComp->GetOwner()->GetComponent<ColliderComponent>()->SetIsDisabled(true);

}

Jotar::AIState* Jotar::OnDamageAIState::OnHandle()
{
	if (m_IsDeath) return nullptr;

	m_DeathTimer += WorldTimeManager::GetInstance().GetDeltaTime();

	if (m_DeathTimer >= m_DeathWaitTime)
	{
		if (m_pAIBehaviorComp->GetOwner()->GetComponent<HealthComponent>()->GetHealth() <= 0)
		{
			m_IsDeath = true;
			m_pAIBehaviorComp->GetAnimatorController()->SetDeathAnimation();
			m_pAIBehaviorComp->GetAnimatorController()->SetDestroyOnLastFrame(true);
			m_pAIBehaviorComp->GetSubject()->NotifyObservers(AIDeathEvent(m_Attacker));
		}
		else
		{
			m_pAIBehaviorComp->GetOwner()->GetComponent<ColliderComponent>()->SetIsDisabled(false);
			return m_pAIBehaviorComp->GetCalculateRandomPathState();
		}
	}

	return nullptr;
}

void Jotar::OnDamageAIState::OnExit()
{
}

void Jotar::OnDamageAIState::Initialize(GameObject* attacker, float deathWaitTime)
{
	m_Attacker = attacker;
	m_DeathWaitTime = deathWaitTime;
}

