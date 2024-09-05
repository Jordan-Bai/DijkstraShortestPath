#include "States.h"

#include "Agent.h"
#include "raylib-cpp.hpp"

std::vector<Transition> State::GetTransitions()
{
    return m_transitions;
}

void State::AddTransition(Transition transition)
{
    m_transitions.push_back(transition);
}

void State::AddTransition(Condition* condition, State* targetState)
{
    Transition newTransition(condition, targetState);
    m_transitions.push_back(newTransition);
}

void GoToPoint::Update(Agent* agent, float deltaTime)
{
    if (IsMouseButtonPressed(0))
    {
        Vector2 mousePos = GetMousePosition();
        agent->GoTo(mousePos.x, mousePos.y);
    }
}

void Wander::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If the agent has reached its destination, go to a random new location
    {
        Node* target = agent->GetMap()->GetRandomNode();
        agent->GoTo(target);
    }
}

Follow::Follow(Agent* target)
	:m_target(target)
{
}

void Follow::Update(Agent* agent, float deltaTime)
{
    if (m_lastTargetNode != m_target->GetCurrentNode()) // If the target has moved, repath to them
    {
        m_lastTargetNode = m_target->GetCurrentNode();
        agent->GoTo(m_lastTargetNode);
    }
}
