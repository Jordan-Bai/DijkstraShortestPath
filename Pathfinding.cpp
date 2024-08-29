#include <Color.hpp>
#include <string>
#include <iostream>
#include <raylib.h>
#include <fstream> // For reading maps from a text file

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
						raylib::Color::Red());
				}
			}
		}
	}
}
