#pragma once

#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include "NavigationSystem.h"

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


	class GoToTargetAIState final : public AIState
	{
	public:
		GoToTargetAIState(AIBehaviorComponent* pAiComp);

		virtual void OnEnter() override;
		virtual AIState* OnHandle() override;
		virtual void OnExit() override;

		void SetPath(std::vector<glm::vec2>& path);
		std::vector<glm::vec2> GetPath() const;
	private:
		MovementComponent* m_pMovementComp;
		std::vector<glm::vec2> m_Path{};
		glm::vec2 m_CurrentDirection{};
	};


	class CalculateTargetAIState final : public AIState
	{
	public:
		CalculateTargetAIState(AIBehaviorComponent* pAiComp);

		virtual void OnEnter() override;
		virtual AIState* OnHandle() override;

	private:
		int CalculateMinPathLength(glm::ivec2 startIndex, glm::ivec2 endIndex);
		glm::ivec2 GetNextRandomCellIndex(glm::ivec2 cellIndex, int wanderRange);

		std::unique_ptr<NavigationSystem> m_pNavigationSystem;
		bool m_IsPathFound;
	};



	class DeadAIState final : public AIState
	{
	public:
		DeadAIState(AIBehaviorComponent* pAiComp);

		virtual void OnEnter() override;
		virtual AIState* OnHandle() override;
		virtual void OnExit() override;
	};


}