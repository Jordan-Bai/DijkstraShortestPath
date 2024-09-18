#include "SearchParameters.h"

#include <algorithm> // For std::find & heap operations
#include "Agent.h"

FleeParam::FleeParam(Agent* target)
	: m_target(target)
{
}

float FleeParam::Evaluate(Node* node)
{
	// Just return distance from the target (the higher the distance, the better)
	glm::vec2 distance = m_target->GetPosition() - node->m_position;
	return glm::length(distance);
}

float FleeParam::GetCost(Edge* edge)
{
	float cost = edge->m_cost;
	for (Edge e : edge->m_target->m_connections)
	{
		if (e.m_target->m_occupant == m_target) // If an adjacent tile contains the agent being fled from
		{
			cost *= 2; // Make that tile cost more
		}
	}
	return cost;
}


std::vector<Node*> BestPath(Agent* agent, SearchParam* param)
{
	Node* startNode = agent->GetCurrentNode();
	float movesLeftScaled = agent->GetMovesLeft() * agent->GetMap()->GetTileSize();

	if (startNode == nullptr || param == nullptr)
	{
		std::cout << "Invalid target search" << std::endl;
		return {}; // Return an empty vector
	}

	startNode->m_gScore = 0;
	startNode->m_previousNode = nullptr;

	Node* currentBestNode = startNode;
	float currentBestScore = param->Evaluate(startNode);

	std::vector<Node*> openList; // List of nodes to be expanded (we will be treating this vector) as a heap
	std::vector<Node*> closedList; // List of nodes that have already been passed

	openList.push_back(startNode);

	while (!openList.empty()) // While the openList isn't empty
	{
		Node* currentNode = openList.front(); // Get the node at the front of the list

		float score = param->Evaluate(currentNode);
		if (score > currentBestScore) // If this node is a better target, make it the current best node
		{
			currentBestScore = score;
			currentBestNode = currentNode;
		}

		// Update open list:
		//-----------------------------------------------------------------------------------------------------
		std::pop_heap(openList.begin(), openList.end(), greaterComp);
		// ^ Move the first element to the end of the list & reorganise the rest into a heap using the comparison operator
		// (If no comparison is provided, it uses <, which will sort the container into a max heap, but we want a min heap)
		openList.pop_back(); // Remove the last element (the one we moved to the back, aka the current node)
		//-----------------------------------------------------------------------------------------------------

		closedList.push_back(currentNode); // Add current node to closed list

		// For each of the node's connections:
		for (int i = 0; i < currentNode->m_connections.size(); i++)
		{
			Node* targetNode = currentNode->m_connections[i].m_target;
			if (std::find(closedList.begin(), closedList.end(), targetNode) == closedList.end()) // Check that the target is not in the closed list
			{
				float gScore = currentNode->m_gScore + currentNode->m_connections[i].m_cost; // How much it actually costs to move there
				float fScore = currentNode->m_fScore + param->GetCost(&currentNode->m_connections[i]);
				// ^ How much the agent wants to move there (determined by search parameters)

				if (gScore > movesLeftScaled) // If the node is out of the agent's range
				{
					continue; // don't check it
				}

				if (std::find(openList.begin(), openList.end(), targetNode) == openList.end()) // If the target isn't in the open list already
				{
					targetNode->m_gScore = gScore;
					targetNode->m_fScore = fScore;
					targetNode->m_previousNode = currentNode;

					// Update open list:
					//-----------------------------------------------------------------------------------------------------
					openList.push_back(targetNode); // Add the node to the open list
					std::push_heap(openList.begin(), openList.end(), greaterComp);
					// ^ Sorts the element at the back of the container (the node we just added) into the heap using the provided comparison operator
					//-----------------------------------------------------------------------------------------------------
				}
				else if (fScore < targetNode->m_fScore) // If the node IS in the open list already, only change its info if this node's path 
					// better fits the search parameters
				{
					targetNode->m_gScore = gScore;
					targetNode->m_fScore = fScore;
					targetNode->m_previousNode = currentNode;

					// Update open list:
					//-----------------------------------------------------------------------------------------------------
					std::make_heap(openList.begin(), openList.end(), greaterComp);
					// ^ Since we've changed the gScore of one of the nodes, we need to reorganise the heap
					//-----------------------------------------------------------------------------------------------------
				}
			}
		}
	}

	if (currentBestNode == startNode) // If no better node was found
	{
		return {}; // Return an empty vector
	}

	std::vector<Node*> finalPath;
	Node* currentNode = currentBestNode;

	while (currentNode != nullptr)
	{
		finalPath.insert(finalPath.begin(), currentNode);
		currentNode = currentNode->m_previousNode; // Follow the previous nodes to the start of the path
	}

	return finalPath;
}
