#include "NavigationSystem.h"
#include "WorldGrid.h"
#include <memory>

#include "GameManager.h"
#include <iostream>


Jotar::NavigationSystem::NavigationSystem()
{
}


std::vector<glm::vec2> Jotar::NavigationSystem::FindPath(const GridCell& startCell, const GridCell& endCell)
{
	std::vector<glm::vec2> path{};
	std::vector<Node> openList{};
	std::vector<Node> closedList{};

	Node currentNode{};
	float costSoFar{};
	//bool stopCurrentLoop = false;

	float costToMove = 1.f;
	//1.: Create a startNode and add it to the openList to start the while loop

	Node startNode{ startCell.CenterCellPosition};
	startNode.CellIndex = startCell.Index;
	startNode.EstimatedTotalCost = GetHeuristicCost(startCell.CenterCellPosition, endCell.CenterCellPosition);

	openList.emplace_back(startNode);

	auto* worldGrid = GameManager::GetInstance().GetWorldGrid();


	while (!openList.empty())
	{
		//	2.A: Get the node with lowest F score
		//	Set the currentNode to the best Node from the openList(the one with the lowest F - cost)
		currentNode = *std::min_element(openList.begin(), openList.end());

		//	2.B: Check if that node leads to the end node
		//	If so, exit the while loop
		if (currentNode.CellIndex != glm::ivec2{ -1, -1 } && endCell.IsConnected(currentNode.CellIndex))
			break;

		//	2.C: Else, we get all the connected cells of the node
		//	Get all the connections from the currentNode(from 2. A) and loop over them

		for (auto connectedCell : worldGrid->GetConnectedCellsFromIndex(currentNode.CellIndex))
		{

			// if it has an object on it, blocking the path
			if (!connectedCell.ObjectOnCell.expired()) continue;

			//For each of the connections…
			//- Calculate the total cost so far(G - cost) (we will need it further on)
			costSoFar = currentNode.CostSoFar + costToMove;



			// check if the connectedcell exist in the closed and openlist
			auto inClosedList = [&closedList, &connectedCell](const Node& node) {
				return node.CellIndex == connectedCell.Index && std::find(closedList.begin(), closedList.end(), node) != closedList.end();
				};

			auto inOpenList = [&openList, &connectedCell](const Node& node) {
				return node.CellIndex == connectedCell.Index && std::find(openList.begin(), openList.end(), node) != openList.end();
				};

			//checks if there exists any node in the closedList
			//If such a node exists, it means that the current connected cell has already been evaluated
			// and added to the closed list, so the loop continues
			if (std::any_of(closedList.begin(), closedList.end(), inClosedList))
				continue;

			//checks if there exists any node in the openList
			//	If such a node exists, it means that the current connected cell has already been evaluated and added to the open list.
			//	If the existing node in the open list has a cost(CostSoFar) greater than the current calculated cost(costSoFar),
			//  it means the current path to this node is cheaper, so the existing node is removed from the open list(openList) to be replaced by the current node.
			if (std::any_of(openList.begin(), openList.end(), inOpenList))
			{
				auto iter = std::find_if(openList.begin(), openList.end(), inOpenList);
				if (iter != openList.end() && iter->CostSoFar <= costSoFar)
					continue;
				openList.erase(iter);
			}


			//2.F: At this point any expensive connection should be removed(if it existed).We create a new nodeRecord and add it to the openList*/
			Node newNode{};
			newNode.CellIndex = connectedCell.Index;
			newNode.CostSoFar = costSoFar;
			newNode.Position = connectedCell.CenterCellPosition;
			newNode.EstimatedTotalCost = costSoFar + GetHeuristicCost(newNode.Position, endCell.CenterCellPosition);

			openList.emplace_back(newNode);
		}

		//2.G: Remove Node from the openList and add it to the closedList
		//closedList.emplace_back(currentNode);

		//openList.erase(std::remove(openList.begin(), openList.end(), currentNode));


		closedList.emplace_back(currentNode);

		openList.erase(std::remove_if(openList.begin(), openList.end(), [&currentNode](const Node& node) {
			return node.CellIndex == currentNode.CellIndex;
			}), openList.end());
	}
	 

	//	3. Reconstruct path from last connection to start node
	//	Track back from the currentRecord until the node of the record is the startnode of the overall path

	while (currentNode.CellIndex != startNode.CellIndex && currentNode.CellIndex != glm::ivec2{ -1, -1 })
	{
		path.emplace_back(currentNode.Position);

		auto iter = std::find_if(closedList.begin(), closedList.end(), [&currentNode](const Node& node) {
			return GameManager::GetInstance().GetWorldGrid()->GetGridCellByID(currentNode.CellIndex).IsConnected(node.CellIndex);
			});

		if (iter != closedList.end())
			currentNode = *iter;
		else
			break;
	}

	//	Add the startnode’s position to the vPath
	path.emplace_back(startCell.CenterCellPosition);

	std::reverse(path.begin(), path.end());

	return path;
}




float Jotar::NavigationSystem::GetHeuristicCost(glm::vec2 startNodePos, glm::vec2 endNodePos)
{
	glm::vec2 toDestination = endNodePos - startNodePos;
	return HeuristicFunctionChebyshev(abs(toDestination.x), abs(toDestination.y));
}

float Jotar::NavigationSystem::HeuristicFunctionChebyshev(float x, float y)
{
	return std::max(x, y);
}
