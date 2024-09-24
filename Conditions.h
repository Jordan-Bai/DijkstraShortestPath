#pragma once

#include "States.h"

class Agent; // Need to declare agent here, so condition & agent can reference each other
// (agent includes state which includes transition which includes condition so we can't just include the agent header)

class Condition : public Behaviour // Abstract base class for all conditions
{
protected:
	bool m_doInverse; // If true, return the opposite of what it would normally return
};

class CloserThan : public Condition
{
	Agent* m_target; // The target whose distance we're finding
	float m_distance; // The distance being compared against

public:
	CloserThan(Agent* target, float distance, bool doInverse);

	bool Execute(Agent* agent) override;
};

class LowHealth : public Condition
{
	int m_hpThreshold; // The HP value being compared against

public:
	LowHealth(int hpThreshold, bool doInverse);

	bool Execute(Agent* agent) override;
};

class HasLineOfSight : public Condition
{
	Agent* m_target; // The target whose distance we're finding

public:
	HasLineOfSight(Agent* target, bool doInverse);

	bool Execute(Agent* agent) override;
};