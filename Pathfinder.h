#pragma once

#include <vector>
#include <glm/glm.hpp> // Using glm for its vectors (specifically vec2)
//#include <fstream> // For reading maps from a text file
#include <iostream>

struct Node;

struct Edge
{
	Node* m_target;
	float m_cost; // Normally just how long it takes to get to the target node

	Edge()
	{
		m_target = nullptr;
		m_cost = 0;
	}
	Edge(Node* target, float cost)
		: m_target(target), m_cost(cost) {}
};

struct Node
{
	glm::vec2 m_position;
	std::vector<Edge> m_connections; // List of paths to other nodes

	Node(float x, float y)
	{
		m_position = { x, y };
	}
	Node(glm::vec2 pos)
		: m_position(pos) {}

	void ConnectTo(Node* other, float cost)
	{
		std::cout << "Connecting nodes" << std::endl;

		if (other == nullptr)
		{
			std::cout << "UH OH" << std::endl;
			return;
		}

		std::cout << other << std::endl;
		std::cout << cost << std::endl;

		m_connections.push_back(Edge(other, cost));

		//Edge myEdge(other, cost);
		//m_connections.push_back(myEdge);

		//m_connections.emplace_back(other, cost);

		std::cout << "Nodes connected" << std::endl;
	}
};

class NodeMap
{
	int m_width, m_height;
	float m_tileSize; // How big do we want each tile to be?

	Node** m_nodes;

public:
	void Initialise(std::vector<std::string> asciiMap, float tileSize); // Creates a Node map from a vector of strings
	//void Inititalise(std::ifstream map); // Creates a Node map from a text file

	Node* GetNode(int x, int y);

	void Draw();
};