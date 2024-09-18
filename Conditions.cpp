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

HPCondition::HPCondition(int hpThreshold, bool lessThanComp)
	: m_hpThreshold(hpThreshold), m_isLessThan(lessThanComp)
{
}

bool HPCondition::IsTrue(Agent* agent)
{
	bool lessThan = (agent->GetHealth() < m_hpThreshold);
	return lessThan == m_isLessThan;
}
