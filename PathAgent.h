#pragma once

#include <vector>
#include <glm/glm.hpp> // Using glm for its vectors (specifically vec2)
#include <Color.hpp>
#include  "Pathfinder.h"

class PathAgent
{
	std::vector<Node*> m_path; // Path it's currently following
	int m_currentIndex;
	Node* m_currentNode;

	glm::vec2 m_position;
	float m_speed = 1; // Speed is 1 by default

public:
	PathAgent() = default;
	PathAgent(Node* node, float speed);

	void GoToNode(Node* node);
	void SetCurrentNode(Node* node);
	Node* GetCurrentNode() const;
	glm::vec2 GetPosition() const;

	void SetSpeed(float speed);
	void SpeedUp();
	void SlowDown();

	bool OnPath();

	void Update(float deltaTime);
	void Draw(Color colour);
};
