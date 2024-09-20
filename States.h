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
	virtual void Enter(Agent* agent) = 0;
	virtual void Update(Agent* agent, float deltaTime) = 0; // "Pure virtual" function
};

class State : public Behaviour // Abstract base class for all states
{
	std::vector<Transition> m_transitions;

public:
	std::vector<Transition> GetTransitions();
	void AddTransition(Transition transition);
	void AddTransition(Condition* condition, State* targetState);

	virtual void Enter(Agent* agent) override; 
	virtual void Update(Agent* agent, float deltaTime) = 0;	
	virtual void Exit(Agent* agent); // For any special behaviours when the state ends
};

class Player : public State
{
public:
	void Update(Agent* agent, float deltaTime) override;

	void Attack(Agent* agent);
};

class MeleeChase : public State
{
	Agent* m_target;

public:
	MeleeChase(Agent* target);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

class MeleeAttack : public State
{
	Agent* m_target;
	
public:
	MeleeAttack(Agent* target);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

class RangedChase : public State
{
	LineOfSightParam m_losParam;

public:
	RangedChase(Agent* target);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

class RangedAttack : public State
{
	Agent* m_target;
	LineOfSightParam m_losParam;

public:
	RangedAttack(Agent* target);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

class Fleeing : public State
{
	FleeParam m_fleeParam;

public:
	Fleeing(Agent* agent);

	void Enter(Agent* agent) override;
	void Update(Agent* agent, float deltaTime) override;
};

//class Damaged : public State // Does the damaged animation
//{
//	float m_maxAnimTime; // How long does the animation last?
//	float m_timer; // How long the animation has left
//
//public:
//	Damaged(float animTime);
//
//	void Enter(Agent* agent) override;
//	void Update(Agent* agent, float deltaTime) override;
//};
