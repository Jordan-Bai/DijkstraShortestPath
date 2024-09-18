#pragma once

class Agent; // Need to declare agent here, so condition & agent can reference each other
// (agent includes state which includes transition which includes condition so we can't just include the agent header)

class Condition // Abstract base class for all conditions
{
public:
	virtual bool IsTrue(Agent* agent) = 0;
};

class DistanceCondition : public Condition
{
	Agent* m_target; // The target whose distance we're finding
	float m_distance; // The distance being compared against
	bool m_isLessThan; // Whether this is a less-than comparision

public:
	DistanceCondition(Agent* target, float distance, bool lessThanComp);

	bool IsTrue(Agent* agent) override;
};

class HPCondition : public Condition
{
	int m_hpThreshold; // The HP value being compared against
	bool m_isLessThan; // Whether this is a less-than comparision

public:
	HPCondition(int hpThreshold, bool lessThanComp);

	bool IsTrue(Agent* agent) override;
};

class LineOfSightCon : public Condition
{
	Agent* m_target; // The target whose distance we're finding

public:
	LineOfSightCon(Agent* target);

	bool IsTrue(Agent* agent) override;
};