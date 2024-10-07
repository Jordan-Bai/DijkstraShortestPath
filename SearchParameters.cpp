#include "SearchParameters.h"

#include <algorithm> // For std::find & heap operations
#include "Agent.h"

float SearchParam::GetHScore(Edge* edge) // Search parameters will return a hScore of 0 by default
{
	return 0;
}


FleeParam::FleeParam(Agent* target)
{
	m_target = target;
}

float FleeParam::Evaluate(Node* node)
{
	// Just return distance from the target (the higher the distance, the better)
	glm::vec2 distance = m_target->GetPosition() - node->m_position;
	return glm::length(distance);
}

bool FleeParam::ValidTarget(Node* node)
{
	if (node->m_occupant == m_target)
	{
		return false; // Means it's not a valid target
	}
	for (Edge e : node->m_connections)
	{
		if (e.m_target->m_occupant == m_target) // If an adjacent tile contains the agent being fled from
		{
			return false; // Don't flee there (don't want to flee to a tile near the agent)
		}
	}
	return true; // Otherwise, any tile is valid
}

float FleeParam::GetHScore(Edge* edge)
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


LineOfSightParam::LineOfSightParam(Agent* target)
{
	m_target = target;
}

float LineOfSightParam::Evaluate(Node* node)
{
	if (node->m_position.x == m_target->GetPosition().x) // If they're on the same x axis (line of sight vertically)
	{
		float tileSize = m_target->GetMap()->GetTileSize();
		int column = node->m_position.x / tileSize;
		int nodeRow = node->m_position.y / tileSize;
		int targetRow = m_target->GetPosition().y / tileSize;

		if (nodeRow < targetRow) // If the node is above the target
		{
			for (int i = nodeRow + 1; i < targetRow; i++) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(column, i);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					return 0;
				}
			}
		}
		else // If the node is below the target
		{
			for (int i = nodeRow - 1; i > targetRow; i--) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(column, i);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					return 0;
				}
			}
		}
	}
	else if (node->m_position.y == m_target->GetPosition().y) // If they're on the same y axis (line of sight horizontally)
	{
		float tileSize = m_target->GetMap()->GetTileSize();
		int row = node->m_position.y / tileSize;
		int nodeColumn = node->m_position.x / tileSize;
		int targetColumn = m_target->GetPosition().x / tileSize;

		if (nodeColumn < targetColumn) // If the node is left of the target
		{
			for (int i = nodeColumn + 1; i < targetColumn; i++) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(i, row);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall), no line of sight
				{
					return 0;
				}
				//if (node->m_occupant && node->m_occupant != m_target) // If there's an occupied node, no line of sight
				//{
				//	return 0;
				//}
			}
		}
		else // If the node is right of the target
		{
			for (int i = nodeColumn - 1; i > targetColumn; i--) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(i, row);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					return 0;
				}
			}
		}
	}
	else // No line of sight
	{
		return 0;
	}

	// If nothing returned 0, there must be line of sight
	glm::vec2 distance = m_target->GetPosition() - node->m_position;
	return glm::length(distance); // Return the distance from the target (prefers spots further away)
}

bool LineOfSightParam::ValidTarget(Node* node)
{
	if (node->m_position.x == m_target->GetPosition().x) // If they're on the same x axis (line of sight vertically)
	{
		float tileSize = m_target->GetMap()->GetTileSize();
		int column = node->m_position.x / tileSize;
		int nodeRow = node->m_position.y / tileSize;
		int targetRow = m_target->GetPosition().y / tileSize;

		if (nodeRow < targetRow) // If the node is above the target
		{
			for (int i = nodeRow + 1; i < targetRow; i++) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(column, i);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					return false;
				}
			}
		}
		else // If the node is below the target
		{
			for (int i = nodeRow - 1; i > targetRow; i--) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(column, i);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					return false;
				}
			}
		}
	}
	else if (node->m_position.y == m_target->GetPosition().y) // If they're on the same y axis (line of sight horizontally)
	{
		float tileSize = m_target->GetMap()->GetTileSize();
		int row = node->m_position.y / tileSize;
		int nodeColumn = node->m_position.x / tileSize;
		int targetColumn = m_target->GetPosition().x / tileSize;

		if (nodeColumn < targetColumn) // If the node is left of the target
		{
			for (int i = nodeColumn + 1; i < targetColumn; i++) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(i, row);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					return false;
				}
			}
		}
		else // If the node is right of the target
		{
			for (int i = nodeColumn - 1; i > targetColumn; i--) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(i, row);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					return false;
				}
			}
		}
	}
	else // No line of sight
	{
		return false;
	}

	return true; // If nothing returned false, there must be line of sight
}


std::vector<Node*> BestTarget(Agent* agent, SearchParam* param)
{
	Node* startNode = agent->GetCurrentNode();
	float movesLeftScaled = agent->GetMovesLeft() * agent->GetMap()->GetTileSize();

	if (startNode == nullptr || param == nullptr)
	{
		std::cout << "Invalid target search" << std::endl;
		return {}; // Return an empty vector
	}

	agent->GetCurrentNode()->m_occupant = nullptr; // Clear the agent from the node so it doesn't get in the way of pathfinding

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
		// If this node is a better target, make it the current best node
		if (score > currentBestScore 
			&& !currentNode->m_occupant) // Can't already have an occupant
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

				if (gScore > movesLeftScaled) // If the node is out of the agent's range
				{
					continue; // don't check it
				}

				if (std::find(openList.begin(), openList.end(), targetNode) == openList.end()) // If the target isn't in the open list already
				{
					targetNode->m_gScore = gScore;
					targetNode->m_hScore = param->GetHScore(&currentNode->m_connections[i]);
					// ^ How much the agent wants to move there (determined by search parameters)
					targetNode->m_fScore = gScore + targetNode->m_hScore;
					targetNode->m_previousNode = currentNode;

					// Update open list:
					//-----------------------------------------------------------------------------------------------------
					openList.push_back(targetNode); // Add the node to the open list
					std::push_heap(openList.begin(), openList.end(), greaterComp);
					// ^ Sorts the element at the back of the container (the node we just added) into the heap using the provided comparison operator
					//-----------------------------------------------------------------------------------------------------
				}
				else if (gScore < targetNode->m_gScore) // If the node IS in the open list already, only change its info if this node's path 
					// better fits the search parameters
				{
					targetNode->m_gScore = gScore;
					targetNode->m_fScore = gScore + targetNode->m_hScore;
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

	agent->GetCurrentNode()->m_occupant = agent; // Put the agent back before returning anything

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

std::vector<Node*> ClosestTarget(Agent* agent, SearchParam* param)
{
	Node* startNode = agent->GetCurrentNode();

	if (startNode == nullptr || param == nullptr)
	{
		std::cout << "Invalid target search" << std::endl;
		return {}; // Return an empty vector
	}
	if (param->Evaluate(startNode)) // If the starting node fulfills the parameters
	{
		return {}; // Return an empty vector
	}

	// Reset the start and end nodes, so they won't retain info from previous searches
	startNode->m_gScore = 0;
	startNode->m_previousNode = nullptr;

	std::vector<Node*> openList; // List of nodes to be expanded (we will be treating this vector) as a heap
	std::vector<Node*> closedList; // List of nodes that have already been passed

	openList.push_back(startNode);

	Node* targetNode = nullptr;

	while (!openList.empty()) // While the openList isn't empty
	{
		Node* currentNode = openList.front(); // Get the node at the front of the list
		if (param->Evaluate(currentNode)) // If we've reached a node that fulfills the parameters, we can end the loop
		{
			targetNode = currentNode;
			if (!currentNode->m_occupant || currentNode->m_occupant == agent)
			{
				break;
			}
			// If the node is occupied by an agent other than the one searching, this isn't really a valid node. However, if there are
			// no other valid nodes, it's still worth pathing towards. Therefore we set this as the target node but continue searching
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

			// Check if edge can be crossed
			if (currentNode->m_connections[i].m_cost > agent->GetMaxMoveScaled())
			{
				// If the cost of moving across this edge is greater than the agent's max movement, the agent will never be able to cross this tile
				continue;
			}

			if (std::find(closedList.begin(), closedList.end(), targetNode) == closedList.end()) // Check that the target is not in the closed list
			{
				float gScore = currentNode->m_gScore + currentNode->m_connections[i].m_cost;
				// ^ gScore is the current node's gScore + the cost to travel to the target node

				if (std::find(openList.begin(), openList.end(), targetNode) == openList.end()) // If the target isn't in the open list already
				{
					targetNode->m_gScore = gScore;
					targetNode->m_hScore = param->GetHScore(&currentNode->m_connections[i]);
					// ^ How much the agent wants to move there (determined by search parameters)
					targetNode->m_fScore = gScore + targetNode->m_hScore;
					targetNode->m_previousNode = currentNode;

					// Update open list:
					//-----------------------------------------------------------------------------------------------------
					openList.push_back(targetNode); // Add the node to the open list
					std::push_heap(openList.begin(), openList.end(), greaterComp);
					// ^ Sorts the element at the back of the container (the node we just added) into the heap using the provided comparison operator
					//-----------------------------------------------------------------------------------------------------
				}
				else if (gScore < targetNode->m_gScore) // If the node IS in the open list already, only change its info if this node's path is faster
				{
					targetNode->m_gScore = gScore;
					targetNode->m_fScore = gScore + targetNode->m_hScore;
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

	if (targetNode == nullptr) // If no valid target was found
	{
		std::cout << "No target found" << std::endl;
		return {}; // Return an empty vector
	}

	std::vector<Node*> finalPath;

	while (targetNode != nullptr)
	{
		finalPath.insert(finalPath.begin(), targetNode);
		targetNode = targetNode->m_previousNode; // Follow the previous nodes to the start of the path
	}

	return finalPath;
}
