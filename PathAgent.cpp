#include "PathAgent.h"

#include <raylib.h>

PathAgent::PathAgent()
	: m_speed(1) // Speed is 1 by default to stop agents from being unable to move
{
}

PathAgent::PathAgent(Node* node, float speed)
	: m_currentNode(node), m_position(node->m_position), m_speed(speed)
{
}


void PathAgent::GoToNode(Node* node, int maxMoveScaled)
{
	m_path = PathSearch(m_currentNode, node, maxMoveScaled);
	m_currentIndex = 0;
}

void PathAgent::FollowPath(std::vector<Node*> path)
{
	m_path = path;
	m_currentIndex = 0;
}

void PathAgent::SetCurrentNode(Node* node)
{
	if (node == nullptr)
	{
		return;
	}
	m_currentNode = node;
	m_position = node->m_position;
}

Node* PathAgent::GetCurrentNode() const
{
	return m_currentNode;
}

glm::vec2 PathAgent::GetPosition() const
{
	return m_position;
}


void PathAgent::SetSpeed(float speed)
{
	m_speed = speed;
}

void PathAgent::SpeedUp()
{
	m_speed *= 2;
}

void PathAgent::SlowDown()
{
	m_speed *= 0.5f;
}


bool PathAgent::OnPath() const
{
	return !m_path.empty(); // Returns whether the path agent is following a path
}


void PathAgent::Update(float deltaTime)
{
	if (m_path.empty())
	{
		return; // If the path is empty, don't do anything
	}

	Node* targetNode = m_path[m_currentIndex + 1];
	glm::vec2 direction = targetNode->m_position - m_position;

	// Get the higher tile cost of the current & target tile
	int maxCost = m_currentNode->m_tileCost;
	if (targetNode->m_tileCost > maxCost)
	{
		maxCost = targetNode->m_tileCost;
	}

	float actualSpeed = m_speed * deltaTime / maxCost; // Adjust speed to match the cost of the tiles we're traversing
	float distance = glm::length(direction) - actualSpeed; // Accounts for overshooting the node

	if (distance > 0)
	{
		// Move the agent
		glm::vec2 vel = glm::normalize(direction) * actualSpeed;
		m_position += vel;
	}
	else // Means we've passed the next node
	{
		// Overshoot protection (mainly for if the agent is going extremely fast)
		//-----------------------------------------------------------------------------------------------------
		bool goToNext = true; // Should we target the next node?
		while (goToNext)
		{
			// Loop through each node in the path till we find a node we haven't passed yet OR a node in a different direction to where we're heading
			if (m_currentIndex < m_path.size() - 2) // If the agent isn't at the final node, go to the next index
			{
				m_currentIndex++;
				m_currentNode = m_path[m_currentIndex];

				Node* nextNode = m_path[m_currentIndex + 1];
				glm::vec2 nextDirection = nextNode->m_position - m_position;

				if (glm::normalize(nextDirection) == glm::normalize(direction)) // If the next target is in the direction we're currently going
				{
					float nextDistance = glm::length(nextDirection) - actualSpeed;
					if (nextDistance > 0) // Means we won't pass the next node this frame
					{
						// If it's going in the correct direction, but it hasn't passed its next target node, move it to its next position
						// and end the loop
						glm::vec2 vel = glm::normalize(nextDirection) * actualSpeed;
						m_position += vel;
						goToNext = false; // EXIT LOOP
					}
					// If nextDistance < 0, means we're going to overshoot the NEXT node as well, so continue the loop
				}
				else // If the next target ISN'T in the direction we're currently going
				{
					// If we move the agent to its next position, it won't be on the path
					m_position = m_currentNode->m_position; // Instead, set the agent's position to their current node, so they're back on the path
					goToNext = false; // EXIT LOOP
				}
			}
			else // If they ARE at the last node, finish the path
			{
				m_currentNode = m_path[m_currentIndex + 1];
				m_path.clear();
				m_position = m_currentNode->m_position;
				goToNext = false; // EXIT LOOP
			}
		}
		//-----------------------------------------------------------------------------------------------------
	}
}

void PathAgent::Draw(Color colour)
{
	// Draw the path
	if (!m_path.empty())
	{
		for (int i = m_currentIndex; i < m_path.size() - 1; i++)
		{
			DrawLine(m_path[i]->m_position.x, m_path[i]->m_position.y, // Node i's position
				m_path[i + 1]->m_position.x, m_path[i + 1]->m_position.y, // Node i+1's position
				colour);
		}
	}

	// Draw the agent
	DrawCircle(m_position.x, m_position.y, 10, colour);
}
