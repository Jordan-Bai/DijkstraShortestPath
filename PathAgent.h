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
	float m_speed;

public:
	PathAgent();
	PathAgent(Node* node, float speed);

	void GoToNode(Node* node, int maxMoveScaled);
	void FollowPath(std::vector<Node*> path);
	void SetCurrentNode(Node* node);
	Node* GetCurrentNode() const;
	glm::vec2 GetPosition() const;

	void SetSpeed(float speed);
	void SpeedUp();
	void SlowDown();

	bool OnPath() const;

	void Update(float deltaTime);
	void Draw(Color colour);
};
