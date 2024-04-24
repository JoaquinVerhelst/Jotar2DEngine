#include "NavigationSystem.h"
#include "WorldGrid.h"
#include <memory>

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
	bool stopCurrentLoop = false;

	float costToMove = 1.f;
	//1.: Create a startNode and add it to the openList to start the while loop

	Node startNode{ startCell.CenterCellPosition};
	startNode.CellIndex = startCell.Index;
	startNode.EstimatedTotalCost = GetHeuristicCost(startCell.CenterCellPosition, endCell.CenterCellPosition);

	openList.emplace_back(startNode);

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

		for (auto connectedCell : WorldGrid::GetInstance().GetConnectedCellsFromIndex(currentNode.CellIndex))
		{
			// if it has an object on it, blocking the path
			if (!connectedCell.ObjectOnCell.expired()) continue;

			//For each of the connections…
			//- Calculate the total cost so far(G - cost) (we will need it further on)
			costSoFar = currentNode.CostSoFar + costToMove;

			//2.D: Check if any of those connections lead to a node already on the closed list
			for (size_t i = 0; i < closedList.size(); i++)
			{
				//Check the closed list(pNode) and if This connection already points to a previously visited Node (already exist in the closed list)
				if (connectedCell.IsConnected(closedList[i].CellIndex))
				{
					//Check if the already existing connection is cheaper(tip: use calculated G - Cost)
					//If so, continue to the next connection
					if (closedList[i].CostSoFar > costSoFar)
					{
						//Else remove it from the closedList
						//(so it can be replaced)
						closedList.erase(std::remove(closedList.begin(), closedList.end(), closedList[i]));
					}
					else
						stopCurrentLoop = true;
				}
			}
			if (stopCurrentLoop)
			{
				stopCurrentLoop = false;
				continue;
			}

			//2.E: If 2.D check failed, check if any of those connections lead to a node already on the open list
			for (size_t i = 0; i < openList.size(); i++)
			{
				//Check the open list(pNode) and if This connection already points to a previously visited Node (already exist in the open list)
				if (connectedCell.IsConnected(openList[i].CellIndex))
				{
					//Check if the already existing connection is cheaper(tip: use calculated G - Cost)
					//If so, continue to the next connection
					if (openList[i].CostSoFar > costSoFar)
					{
						//Else remove it from the openList(so it can be replaced)
						openList.erase(std::remove(openList.begin(), openList.end(), openList[i]));
					}
					else
						stopCurrentLoop = true;
				}
			}
			if (stopCurrentLoop)
			{
				stopCurrentLoop = false;
				continue;
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
		closedList.emplace_back(currentNode);

		openList.erase(std::remove(openList.begin(), openList.end(), currentNode));
	}


	//	3. Reconstruct path from last connection to start node
	//	Track back from the currentRecord until the node of the record is the startnode of the overall path

	while (currentNode.CellIndex != glm::ivec2{-1, -1} && currentNode.CellIndex != startNode.CellIndex)
	{
		//	Add each time the node of the currentRecord to the path
		path.emplace_back(currentNode.Position);

		for (size_t i = 0; i < closedList.size(); ++i)
		{
			if (WorldGrid::GetInstance().GetGridCellByID(currentNode.CellIndex).IsConnected(closedList[i].CellIndex))
			{
				currentNode = closedList[i];
			}
		}
	}

	//	Add the startnode’s position to the vPath
	path.emplace_back(startCell.CenterCellPosition);

	std::reverse(path.begin(), path.end());

	return path;
}




float Jotar::NavigationSystem::GetHeuristicCost(glm::vec2 startNodePos, glm::vec2 endNodePos) const
{
	glm::vec2 toDestination = endNodePos - startNodePos;
	return HeuristicFunctionChebyshev(abs(toDestination.x), abs(toDestination.y));
}

float Jotar::NavigationSystem::HeuristicFunctionChebyshev(float x, float y) const
{
	return std::max(x, y);
}
