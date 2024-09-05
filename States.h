#pragma once

#include "Pathfinder.h"
#include "Conditions.h"

class Agent; // Need to declare agent here, so state & agent can reference each other
class State; // Need to declare state here, so state & transition can reference each other

struct Transition
{
	Condition* m_condition;
	State* m_targetState;

	Transition(Condition* condition, State* targetState)
		: m_condition(condition), m_targetState(targetState) {}
	~Transition()
	{
		delete m_condition;
	}
};

class Behaviour // Abstract base class for states & fsm (so an agent can take a single state OR an fsm as its behaviour);
{
public:
	virtual void Update(Agent* agent, float deltaTime) = 0; // "Pure virtual" function
};

class State : public Behaviour // Abstract base class for all states
{
	std::vector<Transition> m_transitions;

public:
	std::vector<Transition> GetTransitions();
	void AddTransition(Transition transition);
	void AddTransition(Condition* condition, State* targetState);

	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime) = 0;
	virtual void Exit(Agent* agent);
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
