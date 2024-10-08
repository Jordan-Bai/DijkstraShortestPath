#pragma once

#include <vector>
#include <glm/glm.hpp> // Using glm for its vectors (specifically vec2)
#include <iostream>

class Agent; // Need to declare Agent here, so Node & Agent can reference each other
struct Node; // Need to declare Node here, so Node & Edge can reference each other

struct Edge
{
	Node* m_target;
	float m_cost; // Normally just how far away the target node is

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

	// Used for search:
	int m_tileCost;
	float m_gScore; // Represents the cost of the path travelled to get here
	float m_hScore; // Represents the cost of the fastest possible path to the end
	float m_fScore; // gScore + hScore
	Node* m_previousNode;

	Agent* m_occupant = nullptr; // An agent currently in the tile (for making sure only 1 agent occupies a tile at a time

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
			std::cout << "Invalid connection" << std::endl;
			return;
		}

		m_connections.push_back(Edge(other, cost));
	}
};

class NodeMap
{
	int m_width, m_height;
	float m_tileSize;

	Node** m_nodes;

	const char m_wallTile = '#'; // Character that represents a wall tile

public:
	NodeMap() = default;
	// Nodemap doesn't need to be copied, so don't let things use a copy constructor, as that could break things upon destruction
	NodeMap(const NodeMap& other) = delete;
	~NodeMap();

	void Initialise(std::vector<std::string> asciiMap, glm::vec2 screenSize); // Creates a Node map from a vector of strings
	void Initialise(std::string fileName, glm::vec2 screenSize); // Creates a Node map from a text file

	Node* GetNode(int column, int row) const; // Gets the node from a row & column
	Node* GetNearestNode(int x, int y) const; // Gets the node from a screen position
	Node* GetRandomNode() const;

	float GetTileSize() const;
	int GetWidth() const;
	int GetHeight() const;

	void Draw();

	NodeMap* operator=(const NodeMap& other) = delete; // Same as copy constructor
};

inline bool greaterComp(const Node* node1, const Node* node2) // Need to use this as the comparison because otherwise we couldn't compare pointers
{
	return node1->m_fScore > node2->m_fScore;
}

inline float GetHScore(const Node* startNode, const Node* endNode)
{
	glm::vec2 distance = endNode->m_position - startNode->m_position;
	return glm::length(distance);
}

std::vector<Node*> PathSearch(Node* startNode, Node* endNode, int maxMoveScaled); // Declare it here so it can be referenced easier
