#include "Conditions.h"

#include "Agent.h" // Need to include this so agent is properly defined and not just declared

CombinedCon::CombinedCon(Condition* con1, Condition* con2, bool doInverse)
	: m_con1(con1), m_con2(con2)
{
	m_doInverse = doInverse;
}

bool CombinedCon::IsTrue(Agent* agent)
{
	bool combined = m_con1->IsTrue(agent) && m_con2->IsTrue(agent);
	return combined != m_doInverse;
}



CloserThan::CloserThan(Agent* target, float distance, bool doInverse)
	: m_target(target), m_distance(distance* target->GetMap()->GetTileSize())
{
	m_doInverse = doInverse;
}

bool CloserThan::IsTrue(Agent* agent)
{
	bool lessThan = glm::distance(agent->GetPosition(), m_target->GetPosition()) < m_distance;
	return lessThan != m_doInverse;
}


LowHealth::LowHealth(int hpThreshold, bool doInverse)
	: m_hpThreshold(hpThreshold)
{
	m_doInverse = doInverse;
}

bool LowHealth::IsTrue(Agent* agent)
{
	bool lessThan = (agent->GetHealth() < m_hpThreshold);
	return lessThan != m_doInverse;
}


HasLineOfSight::HasLineOfSight(Agent* target, bool doInverse)
	: m_target(target)
{
	m_doInverse = doInverse;
}

bool HasLineOfSight::IsTrue(Agent* agent)
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
	return hasLOS != m_doInverse;
}

