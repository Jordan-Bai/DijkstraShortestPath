#pragma once
#include <vector>
#include <glm/glm.hpp> // Using glm for its vectors (specifically vec2)
#include  "Pathfinder.h"

class PathAgent
{
	std::vector<Node*> m_path; // Path it's currently following
	int m_currentIndex;
	Node* m_currentNode;

	glm::vec2 m_position;
	float m_speed;

public:
	PathAgent();
	PathAgent(Node* node, float speed);

	void GoToNode(Node* node);
	void SetCurrentNode(Node* node);

	void Update(float deltaTime);
	void Draw();
};

