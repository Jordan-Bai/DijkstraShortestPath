#include "Conditions.h"

DistanceCondition::DistanceCondition(Agent* target, float distance, bool lessThanComp)
	: m_target(target), m_distance(distance), m_isLessThan(lessThanComp)
{
}

bool DistanceCondition::IsTrue(Agent* agent)
{
	bool lessThan = (glm::distance(agent->GetPosition(), m_target->GetPosition())) < m_distance;
	return (lessThan == m_isLessThan);
}
