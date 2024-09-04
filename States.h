#pragma once

#include "Pathfinder.h"

class Agent; // Need to declare agent here, so state & agent can reference each other

class State // Abstract base class for all states
{
public:
	virtual void Update(Agent* agent, float deltaTime) = 0; // "Pure virtual" function
};

class GoToPoint : public State
{
public:
	virtual void Update(Agent* agent, float deltaTime);
};

class Wander : public State
{
public:
	virtual void Update(Agent* agent, float deltaTime);
};

class Follow : public State
{
	Agent* m_target;
	Node* m_lastTargetNode;
public:
	Follow(Agent* target);

	virtual void Update(Agent* agent, float deltaTime);
};
