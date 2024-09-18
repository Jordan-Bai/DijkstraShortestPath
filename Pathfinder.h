#pragma once

#include <vector>
#include <glm/glm.hpp> // Using glm for its vectors (specifically vec2)
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
	Edge(const Edge &other)
	{
		m_target = other.m_target;
		m_cost = other.m_cost;
	}
};

struct Node
{
	glm::vec2 m_position;
	std::vector<Edge> m_connections; // List of paths to other nodes

	int m_tileCost;

	// Used for search:
	float m_gScore; // Represents the cost of the path travelled to get here
	Node* m_previousNode;

	Node(float x, float y)
	{
		m_position = { x, y };
	}
	Node(glm::vec2 pos)
		: m_position(pos) {}
	Node(const Node& other)
	{
		m_position = other.m_position;
	}
	~Node()
	{
		m_connections.clear();
	}

	void ConnectTo(Node* other, float cost)
	{
		if (other == nullptr)
		{
			std::cout << "UH OH" << std::endl;
			return;
		}

		m_connections.push_back(Edge(other, cost));
	}
};

class NodeMap
{
	int m_width, m_height;
	float m_tileSize; // How big do we want each tile to be?

	Node** m_nodes;

	const char m_wallTile = '#'; // Character that represents a wall tile

public:
	void Initialise(std::vector<std::string> asciiMap, glm::vec2 screenSize); // Creates a Node map from a vector of strings
	void Initialise(std::string fileName, glm::vec2 screenSize); // Creates a Node map from a text file

	Node* GetNode(int x, int y) const;
	Node* GetNearestNode(int x, int y) const; // Gets the node from a screen position

	void Draw();
};

std::vector<Node*> PathSearch(Node* startNode, Node* endNode);

inline bool greaterComp(const Node* node1, const Node* node2) // Need to use this as the comparison because otherwise we couldn't compare pointers
{
	return node1->m_gScore > node2->m_gScore;
}