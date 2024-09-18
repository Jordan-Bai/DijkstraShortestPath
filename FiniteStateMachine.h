#pragma once

//#include <vector>
#include "States.h"


class FiniteStateMachine : public Behaviour
{
	State* m_lastState;
	State* m_currentState;

public:
	FiniteStateMachine(State* state);

	void Update(Agent* agent, float deltaTime) override;
};

