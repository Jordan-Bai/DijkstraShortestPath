#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine(State* state)
	: m_currentState(state)
{
}

void FiniteStateMachine::Update(Agent* agent, float deltaTime)
{
	State* nextState = nullptr;

	for (Transition t : m_currentState->GetTransitions())
	{
		if (t.m_condition->IsTrue(agent))
		{
			nextState = t.m_targetState;
		}
	}

	if (nextState) // If a new state was chosen
	{
		// Update the current & last state
		m_lastState = m_currentState;
		m_currentState = nextState;

		m_lastState->Exit(agent);
		m_currentState->Enter(agent);
	}

	m_currentState->Update(agent, deltaTime);
}
