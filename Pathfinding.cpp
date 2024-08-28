#include <Color.hpp>
#include <string>
#include <iostream>
#include <raylib.h>

#include "Pathfinder.h"

void NodeMap::Initialise(std::vector<std::string> asciiMap, float tileSize)
{
	std::cout << "Initialising map" << std::endl;
	const char emptyTile = '.'; // Character that represents an empty tile

	m_height = asciiMap.size(); // The number of strings in the vector, aka the number of lines
	m_width = asciiMap[0].size(); // The number of characters in this string (assuming it's the same for each line)
	m_tileSize = tileSize; // The size of each tile

	m_nodes = new Node * [m_width * m_height]; // Create a node pointer for each tile in the map

	// Create the nodes
	std::cout << "Creating Nodes" << std::endl;
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
			else // If the character ISN'T in the line, just put an empty space there
			{
				tile = emptyTile;
			}

			if (tile == emptyTile) // If the tile is empty, it can be pathed through, so add a node there
			{
				m_nodes[x + (y * m_width)] = new Node((x + 0.5f) * m_tileSize, (y + 0.5f) * m_tileSize); // Create a new node at that location
			}
		}
	}

	// Create the connections between nodes
	std::cout << "Creating connections" << std::endl;
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
					std::cout << "Connected west" << std::endl;
				}
				std::cout << "Finished west" << std::endl;

				Node* nodeSouth = GetNode(x, y - 1);
				if (nodeSouth != nullptr)
				{
					// Create connections running both ways
					currentNode->ConnectTo(nodeSouth, 1);
					nodeSouth->ConnectTo(currentNode, 1);
					std::cout << "Connected south" << std::endl;
				}
				std::cout << "Finished south" << std::endl;
			}
		}
	}
}

//void NodeMap::Inititalise(std::ifstream map)
//{
//	std::string line;
//	int height;
//	while (std::getline(map, line)) // While there are still lines in the map
//	{
//		
//	}
//}

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
	std::cout << "Drawing" << std::endl;
	for (int y = 0; y < m_height; y++) // For each line in the map (aka each y value)
	{
		for (int x = 0; x < m_width; x++) // For each character in the line (aka each x value)
		{
			Node* currentNode = GetNode(x, y);
			if (currentNode == nullptr)
			{
				DrawRectangle(x * m_tileSize, y * m_tileSize, m_tileSize, m_tileSize, raylib::Color::Red());
			}
		}
	}
}
