#pragma once
#include <vector>
#include "glm/glm.hpp"



namespace Jotar
{

	struct GridCell;

	// This Navigation System is based on the A* algorithm framework we made in Gameplay Programming
	class NavigationSystem final
	{
		struct Node
		{
			Node() {};
			Node(glm::vec2 startPos)
				: Position{ startPos } {}

			float CostSoFar = 0.f; 	// accumulated g-costs of all the connections leading up to this one
			float EstimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)
			glm::vec2 Position{};
			glm::ivec2 CellIndex{-1, -1};

			bool operator==(const Node& other) const
			{
				return Position == other.Position
					&& CostSoFar == other.CostSoFar
					&& EstimatedTotalCost == other.EstimatedTotalCost;
			};

			bool operator<(const Node& other) const
			{
				return EstimatedTotalCost < other.EstimatedTotalCost;
			};
		};

	public:
		NavigationSystem() = default;
		~NavigationSystem() = default;

		NavigationSystem(const NavigationSystem& other) = delete;
		NavigationSystem(NavigationSystem&& other) = delete;
		NavigationSystem& operator=(const NavigationSystem& other) = delete;
		NavigationSystem& operator=(NavigationSystem&& other) = delete;


		std::vector<glm::vec2> FindPath(const GridCell& startCell, const GridCell& endCell);
	private:

		float GetHeuristicCost(glm::vec2 startNodePos, glm::vec2 endNodePos);

		float HeuristicFunctionChebyshev(float x, float y);
	};
}
