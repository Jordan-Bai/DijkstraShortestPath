#include <Color.hpp>
#include <string>
#include <iostream>
#include <raylib.h>
#include <fstream> // For reading maps from a text file
#include <algorithm> // For std::find & heap operations

#include "Pathfinder.h"

void NodeMap::Initialise(std::vector<std::string> asciiMap, glm::vec2 screenSize)
{
	m_height = asciiMap.size(); // The number of strings in the vector, aka the number of lines
	m_width = asciiMap[0].size(); // The number of characters in this string (assuming it's the same for each line)

	float yMax = screenSize.y / m_height;
	float xMax = screenSize.x / m_width;
	m_tileSize = yMax < xMax ? yMax : xMax; // Set whichever is smaller as the tile size

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
			else // If the character DOESN'T exist, just put a 1 cost space there
			{
				tile = '1';
			}

			if (tile != m_wallTile) // If the tile isn't a wall, it can be pathed through, so add a node there
			{
				m_nodes[x + (y * m_width)] = new Node((x + 0.5f) * m_tileSize, (y + 0.5f) * m_tileSize); // Create a new node at that location

				int tileCost = tile - '0'; // By subtracting the character for 0, we can convert the ascii value of a number to what number it is
				if (tileCost > 0 && tileCost < 10) // Means character was a valid number
				{
					m_nodes[x + (y * m_width)]->m_tileCost = tileCost;
				}
				else // If it's not a recognized character, just put a 1 cost space there
				{
					m_nodes[x + (y * m_width)]->m_tileCost = 1; 
				}
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
					// Get whichever tile cost is higher: That will be the cost of the edges
					int maxCost = currentNode->m_tileCost;
					if (nodeWest->m_tileCost > maxCost)
					{
						maxCost = nodeWest->m_tileCost;
					}

					// Create connections running both ways
					currentNode->ConnectTo(nodeWest, maxCost * m_tileSize);
					nodeWest->ConnectTo(currentNode, maxCost * m_tileSize);
					// ^ Multiply tile cost by tile size, so cost is representative of the distance travelled
				}

				Node* nodeNorth = GetNode(x, y - 1);
				if (nodeNorth != nullptr)
				{
					// Get whichever tile cost is higher: That will be the cost of the edges
					int maxCost = currentNode->m_tileCost;
					if (nodeNorth->m_tileCost > maxCost)
					{
						maxCost = nodeNorth->m_tileCost;
					}

					// Create connections running both ways
					currentNode->ConnectTo(nodeNorth, maxCost * m_tileSize);
					nodeNorth->ConnectTo(currentNode, maxCost * m_tileSize);
				}
			}
		}
	}
}

void NodeMap::Initialise(std::string fileName, glm::vec2 screenSize)
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

	Initialise(fileMap, screenSize); // Have to use the vector version so we can figure out the map height before iterating through it,
	// since we need to actually create m_nodes
}

Node* NodeMap::GetNode(int column, int row) const
{
	if (column < 0 || column >= m_width || row < 0 || row >= m_height)
	{
		return nullptr; // If the tile is outside of the map, return null
	}
	return m_nodes[column + (row * m_width)];
}

Node* NodeMap::GetNearestNode(int x, int y) const
{
	int xCell = x / m_tileSize;
	int yCell = y / m_tileSize;
	return GetNode(xCell, yCell);
}

Node* NodeMap::GetRandomNode() const
{
	Node* node = nullptr;
	while (node == nullptr)
	{
		int x = rand() % m_width;
		int y = rand() % m_height;
		node = GetNode(x, y);
	}
	return node;
}

float NodeMap::GetTileSize() const
{
	return m_tileSize;
}

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
					// Change the line colour depending on the edge's cost
					raylib::Color lineColor;
					if (node->m_connections[i].m_cost == m_tileSize)
					{
						lineColor = raylib::Color::LightGray();
					}
					else if (node->m_connections[i].m_cost <= 3 * m_tileSize)
					{
						lineColor = raylib::Color::Orange();
					}
					else
					{
						lineColor = raylib::Color::Red();
					}
					Node* neighbour = node->m_connections[i].m_target;

					DrawLine(node->m_position.x, node->m_position.y,		// This node's position (start of the line)
						neighbour->m_position.x, neighbour->m_position.y,	// Neighbour's position (end of the line)
						lineColor);
				}
			}
		}
	}
}


std::vector<Node*> PathSearch(Node* startNode, Node* endNode, int maxMoveScaled)
{
	if (startNode == nullptr || endNode == nullptr)
	{
		std::cout << "Invalid path search" << std::endl;
		return {}; // Return an empty vector
	}
	if (startNode == endNode)
	{
		return {}; // Return an empty vector
	}

	// Reset the start and end nodes, so they won't retain info from previous searches
	startNode->m_gScore = 0;
	startNode->m_previousNode = nullptr;
	endNode->m_previousNode = nullptr;

	std::vector<Node*> openList; // List of nodes to be expanded (we will be treating this vector) as a heap
	std::vector<Node*> closedList; // List of nodes that have already been passed

	openList.push_back(startNode);

	while (!openList.empty()) // While the openList isn't empty
	{
		Node* currentNode = openList.front(); // Get the node at the front of the list
		if (currentNode == endNode) // If we've reached the node we're looking for, we can end the loop
		{
			break;
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
			if (currentNode->m_connections[i].m_cost > maxMoveScaled)
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
					targetNode->m_hScore = GetHScore(targetNode, endNode);
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

	if (endNode->m_previousNode == nullptr) // If a path to the end was not found
	{
		std::cout << "No path found" << std::endl;
		return {}; // Return an empty vector
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
