#include "States.h"

#include "Agent.h"
#include "raylib-cpp.hpp"

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
