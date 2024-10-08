#include "Conditions.h"

#include "Agent.h" // Need to include this so agent is properly defined and not just declared


CloserThan::CloserThan(Agent* target, float distance)
	: m_target(target), m_distance(distance* target->GetMap()->GetTileSize())
{
}

bool CloserThan::Execute(Agent* agent)
{
	return glm::distance(agent->GetPosition(), m_target->GetPosition()) < m_distance;
}


LowHealth::LowHealth(int hpThreshold)
	: m_hpThreshold(hpThreshold)
{
}

bool LowHealth::Execute(Agent* agent)
{
	return agent->GetHealth() < m_hpThreshold;
}


HasLineOfSight::HasLineOfSight(Agent* target)
	: m_target(target)
{
}

bool HasLineOfSight::Execute(Agent* agent)
{
	bool hasLOS = true;
	glm::vec2 agentPos = agent->GetPosition();
	if (agentPos.x == m_target->GetPosition().x) // If they're on the same x axis (line of sight vertically)
	{
		float tileSize = m_target->GetMap()->GetTileSize();
		int column = agentPos.x / tileSize;
		int nodeRow = agentPos.y / tileSize;
		int targetRow = m_target->GetPosition().y / tileSize;

		if (nodeRow < targetRow) // If the node is above the target
		{
			for (int i = nodeRow + 1; i < targetRow; i++) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(column, i);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					hasLOS = false;
				}
			}
		}
		else // If the node is below the target
		{
			for (int i = nodeRow - 1; i > targetRow; i--) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(column, i);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					hasLOS = false;
				}
			}
		}
	}
	else if (agentPos.y == m_target->GetPosition().y) // If they're on the same y axis (line of sight horizontally)
	{
		float tileSize = m_target->GetMap()->GetTileSize();
		int row = agentPos.y / tileSize;
		int nodeColumn = agentPos.x / tileSize;
		int targetColumn = m_target->GetPosition().x / tileSize;

		if (nodeColumn < targetColumn) // If the node is left of the target
		{
			for (int i = nodeColumn + 1; i < targetColumn; i++) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(i, row);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					hasLOS = false;
				}
			}
		}
		else // If the node is right of the target
		{
			for (int i = nodeColumn - 1; i > targetColumn; i--) // Check each spot between the node and the target 
			{
				Node* node = m_target->GetMap()->GetNode(i, row);
				if (!node || node->m_occupant) // If there isn't a node (aka there's a wall) or there's an occupied node, no line of sight
				{
					hasLOS = false;
				}
			}
		}
	}
	else // No line of sight
	{
		hasLOS = false;
	}
	return hasLOS;
}


bool IsMoving::Execute(Agent* agent)
{
	return !agent->PathComplete(); // If the path is complete, the agent isn't moving
}


bool CanMove::Execute(Agent* agent)
{
	// If the agent has movement left (doesn't automatically mean they can move, as the adjacent tiles could cost too much)
	if (agent->GetMovesLeft() > 0)
	{
		// Check each tile next to the agent
		std::vector<Edge> neighbours = agent->GetCurrentNode()->m_connections;
		for (Edge e : agent->GetCurrentNode()->m_connections)
		{
			if (e.m_cost <= agent->GetMovesLeftScaled()) //  If one of the adjacent tiles can be moved to
			{
				return true;
			}
		}
	}

	return false; // Otherwise, return false
}


KeyPressed::KeyPressed(KeyboardKey key)
	:m_key(key)
{
}

bool KeyPressed::Execute(Agent* agent)
{
	return IsKeyPressed(m_key);
}


MousePressed::MousePressed(MouseButton button)
	:m_button(button)
{
}

bool MousePressed::Execute(Agent* agent)
{
	return IsMouseButtonPressed(m_button);
}
