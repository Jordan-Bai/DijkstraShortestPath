#include "Conditions.h"

#include "Agent.h" // Need to include this so agent is properly defined and not just declared

DistanceCondition::DistanceCondition(Agent* target, float distance, bool lessThanComp)
	: m_target(target), m_distance(distance), m_isLessThan(lessThanComp)
{
}

bool DistanceCondition::IsTrue(Agent* agent)
{
	bool lessThan = (glm::distance(agent->GetPosition(), m_target->GetPosition())) < m_distance;
	return (lessThan == m_isLessThan);
}

bool FinishedMoving::IsTrue(Agent* agent)
{
	return agent->PathComplete();
}

bool NewTurn::IsTrue(Agent* agent)
{
	return !agent->TurnComplete();
}
