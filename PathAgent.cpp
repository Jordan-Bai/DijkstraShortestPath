#include "PathAgent.h"

#include <Color.hpp>
#include <raylib.h>
#include <iostream>

PathAgent::PathAgent(Node* node, float speed)
	: m_currentNode(node), m_position(node->m_position), m_speed(speed)
{
}

void PathAgent::GoToNode(Node* node)
{
	m_path = PathSearch(m_currentNode, node);
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

void PathAgent::SpeedUp()
{
	m_speed *= 2;
}

void PathAgent::SlowDown()
{
	m_speed *= 0.5f;
}

void PathAgent::Update(float deltaTime)
{
	if (m_path.empty())
	{
		return; // If the path is empty, don't do anything
	}

	//std::cout << "Moving" << std::endl;

	Node* targetNode = m_path[m_currentIndex + 1];
	glm::vec2 direction = targetNode->m_position - m_position;
	float distance = glm::length(direction);

	if (distance > 1)
	{
		// Move the agent
		glm::vec2 vel = glm::normalize(direction) * m_speed * deltaTime;
		m_position += vel;
	}
	else // If the agent is in range of the target node, go to the next one
	{
		if (m_currentIndex < m_path.size() - 2) // If the agent isn't at the final node, go to the next index
		{
			m_currentIndex++;
			m_currentNode = m_path[m_currentIndex];
		}
		else // If they ARE at the end, finish the path
		{
			m_currentNode = m_path[m_currentIndex + 1];
			m_path.clear();
		}
	}
}

void PathAgent::Draw()
{
	// Draw the agent
	DrawCircle(m_position.x, m_position.y, 10, raylib::Color::Black());

	// Draw the path
	if (!m_path.empty())
	{
		for (int i = 0; i < m_path.size() - 1; i++)
		{
			DrawLine(m_path[i]->m_position.x, m_path[i]->m_position.y, // Node i's position
				m_path[i + 1]->m_position.x, m_path[i + 1]->m_position.y, // Node i+1's position
				raylib::Color::Black());

			// For testing
				//-----------------------------------------------------------------------------------------------------
			raylib::Color textColor = raylib::Color::Black();
			std::string text = std::to_string(int(m_path[i + 1]->m_gScore));
			textColor.DrawText(text, m_path[i + 1]->m_position.x, m_path[i + 1]->m_position.y - 10, 10);
			//-----------------------------------------------------------------------------------------------------
		}
	}
}
