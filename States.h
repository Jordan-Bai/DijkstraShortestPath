#pragma once

#include "Pathfinder.h"
#include "Conditions.h"
#include "SearchParameters.h"

class State; // Need to declare state here, so state & transition can reference each other

struct Transition
{
	Condition* m_condition;
	State* m_targetState;

	Transition(Condition* condition, State* targetState)
		: m_condition(condition), m_targetState(targetState) {}
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

	virtual void Enter(Agent* agent); // For any special behaviours when the state begins
	virtual void Update(Agent* agent, float deltaTime) = 0;	
	virtual void Exit(Agent* agent); // For any special behaviours when the state ends
};

class GoToPoint : public State
{
public:
	void Update(Agent* agent, float deltaTime) override;
};

class Wander : public State
{
public:
	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

class Follow : public State
{
	Agent* m_target;
	Node* m_lastTargetNode;

public:
	Follow(Agent* target);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

class Player : public State
{
public:
	void Update(Agent* agent, float deltaTime) override;

	void Attack(Agent* agent);
};

class MeleeAttack : public State
{
	Agent* m_target;
	float m_range; // How close they have to be to attack
	
public:
	MeleeAttack(Agent* target, float range);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;

	void Move(Agent* agent);
	void Attack(Agent* agent);
};

class RangedChase : public State
{
	LineOfSight m_los;

public:
	RangedChase(Agent* target);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

class RangedAttack : public State
{
	LineOfSight m_los;

public:
	RangedAttack(Agent* target);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

class Fleeing : public State
{
	FleeParam m_fleeParam;

public:
	Fleeing(Agent* target);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};
