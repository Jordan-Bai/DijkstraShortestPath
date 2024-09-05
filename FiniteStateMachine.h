#pragma once

//#include <vector>
#include "States.h"


class FiniteStateMachine : public Behaviour
{
	//std::vector<State*> m_states;

	State* m_lastState;
	State* m_currentState;

public:
	FiniteStateMachine();
	FiniteStateMachine(State* state);
	~FiniteStateMachine();

	void Update(Agent* agent, float deltaTime);
};

