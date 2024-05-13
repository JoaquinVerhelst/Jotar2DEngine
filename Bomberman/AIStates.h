#pragma once

#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include "NavigationSystem.h"
#include "ColliderComponent.h"


namespace Jotar
{
	class MovementComponent;
	class AIBehaviorComponent;


	class AIState
	{
	public:
		AIState(AIBehaviorComponent* pAiComp)
			:m_pAIBehaviorComp{ pAiComp }
		{
		}
		virtual ~AIState() = default;

		AIState(AIState&& other) = delete;
		AIState(const AIState& other) = delete;
		const AIState& operator=(AIState&& other) = delete;
		const AIState& operator=(const AIState& other) = delete;

		virtual void OnEnter() {}
		virtual AIState* OnHandle() { return this; }
		virtual void OnExit() {}

	protected:
		AIBehaviorComponent* m_pAIBehaviorComp;
	};


	class IdleAIState final : public AIState
	{
	public:
		IdleAIState(AIBehaviorComponent* pAiComp);

		virtual void OnEnter() override;
		virtual AIState* OnHandle() override;
		virtual void OnExit() override;
	};


	class GoToTargetAIState : public AIState
	{
	public:
		GoToTargetAIState(AIBehaviorComponent* pAiComp);

		virtual AIState* OnHandle() override;
		void SetPath(std::vector<glm::vec2>& path);
		std::vector<glm::vec2> GetPath() const;

		glm::ivec2 GetCurrentDirection();
		void MoveTowardsNextPoint(glm::vec2& pos, glm::vec2& pathPos);

	protected:
		void CalculateDirection(glm::vec2& pos, glm::vec2& pathPos);
		virtual bool CheckDistanceToPoint(glm::vec2& pos, glm::vec2& pathPos);
		void CheckForAnimationUpdate();

		MovementComponent* m_pMovementComp;
		std::vector<glm::vec2> m_Path{};
		glm::ivec2 m_PreviousDirection{};
		glm::ivec2 m_CurrentDirection{};
	};

	class ChaseTargetAIState final : public GoToTargetAIState
	{
	public:
		ChaseTargetAIState(AIBehaviorComponent* pAiComp);

		virtual void OnEnter() override;
		virtual AIState* OnHandle() override;
	private:

		float m_TimerCounter;
		float m_ResetPathTime;
		bool m_ShouldReset;
	};


	class CalculateRandomPathAIState : public AIState
	{
	public:
		CalculateRandomPathAIState(AIBehaviorComponent* pAiComp);
		virtual AIState* OnHandle() override;

	private:
		int CalculateMinPathLength(glm::ivec2 startIndex, glm::ivec2 endIndex);
		glm::ivec2 GetNextRandomCellIndex(glm::ivec2 cellIndex, int wanderRange) const;

	protected:
		bool CalculatePath(const GridCell& startCell, const GridCell& endCell);
		std::unique_ptr<NavigationSystem> m_pNavigationSystem;
		bool m_IsPathFound;
	};

	class CalculatePathToPlayerAIState final : public CalculateRandomPathAIState
	{
	public:
		CalculatePathToPlayerAIState(AIBehaviorComponent* pAiComp);
		virtual AIState* OnHandle() override;

		void SetTarget(ColliderComponent* targetCollider);

	private:
		ColliderComponent* m_pTargetCollider;

	};



	class OnDamageAIState final : public AIState
	{
	public:
		OnDamageAIState(AIBehaviorComponent* pAiComp);

		virtual void OnEnter() override;
		virtual AIState* OnHandle() override;
		virtual void OnExit() override;

	private:
		float m_DeathTimer;
		float m_DeathWaitTime;
		bool m_IsDeath;
	};


}