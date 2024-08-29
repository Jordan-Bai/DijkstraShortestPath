#include <Color.hpp>
#include <string>
#include <iostream>
#include <raylib.h>
#include <fstream> // For reading maps from a text file
#include <algorithm> // For std::find

#include "Pathfinder.h"

void NodeMap::Initialise(std::vector<std::string> asciiMap, float tileSize)
{
	std::cout << "Initialising map" << std::endl;

	m_height = asciiMap.size(); // The number of strings in the vector, aka the number of lines
	m_width = asciiMap[0].size(); // The number of characters in this string (assuming it's the same for each line)
	m_tileSize = tileSize; // The size of each tile

	m_nodes = new Node * [m_width * m_height]; // Create a node pointer for each tile in the map

	// Create the nodes
	for (int y = 0; y < m_height; y++) // For each line in the map (aka each y value)
	{
		std::string& line = asciiMap[y];
		if (line.size() != m_width) // Check if the line is the correct length
		{
			// Print that the line is mismatched
			std::cout << "Line #" << y << " is mismatched(" << line.size() << " instead of" << m_width << ")" << std::endl;
		}

		for (int x = 0; x < m_width; x++) // For each character in the line (aka each x value)
		{
			char tile;
			if (x < line.size()) // If the character we're trying to access actually exists, set it as the tile character
			{
				tile = line[x];
			}
			else // If the character DOESN'T exist, just put an empty space there
			{
				tile = m_emptyTile;
			}

			if (tile == m_emptyTile) // If the tile is empty, it can be pathed through, so add a node there
			{
				m_nodes[x + (y * m_width)] = new Node((x + 0.5f) * m_tileSize, (y + 0.5f) * m_tileSize); // Create a new node at that location
				m_nodes[x + (y * m_width)]->m_x = x;
				m_nodes[x + (y * m_width)]->m_y = y;
			}
			else
			{
				m_nodes[x + (y * m_width)] = nullptr;
			}
		}
	}

	// Create the connections between nodes
	for (int y = 0; y < m_height; y++) // For each line in the map (aka each y value)
	{
		for (int x = 0; x < m_width; x++) // For each character in the line (aka each x value)
		{
			Node* currentNode = GetNode(x, y);
			if (currentNode != nullptr)
			{
				Node* nodeWest = GetNode(x - 1, y);
				if (nodeWest != nullptr)
				{
					// Create connections running both ways
					currentNode->ConnectTo(nodeWest, 1);
					nodeWest->ConnectTo(currentNode, 1);
				}

				Node* nodeSouth = GetNode(x, y - 1);
				if (nodeSouth != nullptr)
				{
					// Create connections running both ways
					currentNode->ConnectTo(nodeSouth, 1);
					nodeSouth->ConnectTo(currentNode, 1);
				}
			}
		}
	}
}

void NodeMap::Initialise(std::string fileName, float tileSize)
{
	std::ifstream ifs(fileName, std::ifstream::in);
	if (!ifs) // Check if a file was loaded
	{
		std::cout << "INVALID FILE - NO MAP CREATED" << std::endl;
		return;
	}

	std::vector<std::string> fileMap;
	std::string line;

	while (std::getline(ifs, line)) // While there are still lines in the map
	{
		fileMap.push_back(line);
	}

	Initialise(fileMap, tileSize); // Have to use the vector version so we can figure out the map height before iterating through it,
	// since we need to actually create m_nodes
}

Node* NodeMap::GetNode(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		return nullptr; // If the tile is outside of the map, return null
	}
	return m_nodes[x + (y * m_width)];
}

Node* NodeMap::GetNearestNode(int x, int y)
{
	int xCell = x / m_tileSize;
	int yCell = y / m_tileSize;
	return GetNode(xCell, yCell);
}

//std::vector<Node*> NodeMap::PathSearch(Node* startNode, Node* endNode)
//{
//	if (startNode == nullptr || endNode == nullptr)
//	{
//		std::cout << "Invalid path search" << std::endl;
//		return {}; // Return an empty vector
//	}
//	if (startNode == endNode)
//	{
//		return {}; // Return an empty vector (CHECK AFTER FINISHING FUNCTION)
//	}
//
//	// Reset the start and end nodes, so they won't retain info from previous searches
//	startNode->m_gScore = 0;
//	startNode->m_previousNode = nullptr;
//	endNode->m_previousNode = nullptr;
//
//	std::vector<Node*> openList; // List of nodes to be expanded
//	std::vector<Node*> closedList; // List of nodes that have already been passed
//
//	openList.push_back(startNode);
//
//	while (!openList.empty()) // While the openList isn't empty
//	{
//		// Sort list by gscore (only necessary if edge cost isn't always the same)
//
//		Node* currentNode = openList.front(); // Get the node at the front of the list
//		if (currentNode == endNode) // If we've reached the node we're looking for, we can end the loop
//		{
//			break;
//		}
//
//		openList.erase(openList.begin()); // Remove the current node (the one at the front) from the open list
//		closedList.push_back(currentNode); // Add current node to closed list
//
//		// For each of the node's connections:
//		for (int i = 0; i < currentNode->m_connections.size(); i++)
//		{
//			Node* targetNode = currentNode->m_connections[i].m_target;
//			if (std::find(closedList.begin(), closedList.end(), targetNode) == closedList.end()) // Check that the target is not in the closed list
//			{
//				float gScore = currentNode->m_gScore + currentNode->m_connections[i].m_cost;
//				// ^ gScore is the current node's gScore + the cost to travel to the target node
//
//				if (std::find(openList.begin(), openList.end(), targetNode) == openList.end()) // If the target isn't in the open list already
//				{
//					targetNode->m_gScore = gScore;
//					targetNode->m_previousNode = currentNode;
//					openList.push_back(targetNode);
//				}
//				else if (gScore < targetNode->m_gScore) // If the node IS in the open list already, only change its info if this node's path is faster
//				{
//					targetNode->m_gScore = gScore;
//					targetNode->m_previousNode = currentNode;
//				}
//			}
//		}
//	}
//
//	if (endNode == nullptr) // If a path to the end was not found
//	{
//		std::cout << "No path found" << std::endl;
//		return {}; // Return an empty vector (CHECK AFTER FINISHING FUNCTION)
//	}
//
//	std::vector<Node*> finalPath;
//	Node* currentNode = endNode;
//
//	while (currentNode != nullptr)
//	{
//		finalPath.push_back(currentNode);
//		currentNode = currentNode->m_previousNode; // Follow the previous nodes to the start of the path
//	}
//
//	return finalPath;
//}

void NodeMap::Draw()
{
	for (int y = 0; y < m_height; y++) // For each line in the map (aka each y value)
	{
		for (int x = 0; x < m_width; x++) // For each character in the line (aka each x value)
		{
			Node* node = GetNode(x, y);
			if (node == nullptr) // If it's a wall, draw a square there
			{
				DrawRectangle(x * m_tileSize, y * m_tileSize, m_tileSize - 1, m_tileSize - 1, raylib::Color::Red());
			}
			else // If not, draw what tiles it's connected to
			{
				for (int i = 0; i < node->m_connections.size(); i++)
				{
					Node* neighbour = node->m_connections[i].m_target;
					DrawLine(node->m_position.x, node->m_position.y,				// This node's position (start of the line)
						neighbour->m_position.x, neighbour->m_position.y,			// Neighbour's position (end of the line)
						raylib::Color::Green());
				}
			}
		}
	}
}

std::vector<Node*> PathSearch(Node* startNode, Node* endNode)
{
	if (startNode == nullptr || endNode == nullptr)
	{
		std::cout << "Invalid path search" << std::endl;
		return {}; // Return an empty vector
	}
	if (startNode == endNode)
	{
		return {}; // Return an empty vector (CHECK AFTER FINISHING FUNCTION)
	}

	// Reset the start and end nodes, so they won't retain info from previous searches
	startNode->m_gScore = 0;
	startNode->m_previousNode = nullptr;
	endNode->m_previousNode = nullptr;

	std::vector<Node*> openList; // List of nodes to be expanded
	std::vector<Node*> closedList; // List of nodes that have already been passed

	openList.push_back(startNode);

	while (!openList.empty()) // While the openList isn't empty
	{
		// Sort list by gscore (only necessary if edge cost isn't always the same)

		Node* currentNode = openList.front(); // Get the node at the front of the list
		if (currentNode == endNode) // If we've reached the node we're looking for, we can end the loop
		{
			break;
		}

		openList.erase(openList.begin()); // Remove the current node (the one at the front) from the open list
		closedList.push_back(currentNode); // Add current node to closed list

		// For each of the node's connections:
		for (int i = 0; i < currentNode->m_connections.size(); i++)
		{
			Node* targetNode = currentNode->m_connections[i].m_target;
			if (std::find(closedList.begin(), closedList.end(), targetNode) == closedList.end()) // Check that the target is not in the closed list
			{
				float gScore = currentNode->m_gScore + currentNode->m_connections[i].m_cost;
				// ^ gScore is the current node's gScore + the cost to travel to the target node

				if (std::find(openList.begin(), openList.end(), targetNode) == openList.end()) // If the target isn't in the open list already
				{
					targetNode->m_gScore = gScore;
					targetNode->m_previousNode = currentNode;
					openList.push_back(targetNode);
				}
				else if (gScore < targetNode->m_gScore) // If the node IS in the open list already, only change its info if this node's path is faster
				{
					targetNode->m_gScore = gScore;
					targetNode->m_previousNode = currentNode;
				}
			}
		}
	}

	if (endNode == nullptr) // If a path to the end was not found
	{
		std::cout << "No path found" << std::endl;
		return {}; // Return an empty vector (CHECK AFTER FINISHING FUNCTION)
	}

	std::vector<Node*> finalPath;
	Node* currentNode = endNode;

	while (currentNode != nullptr)
	{
		finalPath.insert(finalPath.begin(), currentNode);
		currentNode = currentNode->m_previousNode; // Follow the previous nodes to the start of the path
	}

	return finalPath;
}
