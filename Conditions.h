#pragma once

#include "Agent.h"

class Condition
{
public:
	virtual bool IsTrue(Agent* agent) = 0;
};

class DistanceCondition : public Condition
{
	Agent* m_target;
	float m_distance; // The distance being compared against
	bool m_isLessThan; // Whether this is a less-than comparision

public:
	DistanceCondition(Agent* target, float distance, bool lessThanComp);
	virtual bool IsTrue(Agent* agent);
};
