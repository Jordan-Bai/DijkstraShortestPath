#include "States.h"

#include <iostream>
#include "Agent.h"

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

void State::Enter(Agent* agent)
{
}

// REMOVE LATER
//--------------------------------------------------------------------------------------
void State::Update(Agent* agent, float deltaTime)
{
	
}
//--------------------------------------------------------------------------------------

void State::Exit(Agent* agent)
{
}


// GoToPoint
void GoToPoint::Update(Agent* agent, float deltaTime)
{
    if (IsMouseButtonPressed(0))
    {
        Vector2 mousePos = GetMousePosition();
        Node* target = agent->GetMap()->GetNearestNode(mousePos.x, mousePos.y);
        std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), target);
        if (desiredPath.empty()) // If there's no path to the target, don't move
        {
            return;
        }

        float maxMoveScaled = agent->GetMaxMove() * agent->GetMap()->GetTileSize();
        if (target->m_gScore <= maxMoveScaled) // If the target node is in range, go to it
        {
            agent->GoTo(target);
        }
    }
}

// Wander
void Wander::Enter(Agent* agent)
{
    // Make agent blue when wandering
    agent->SetColour({ 0, 0, 255, 255 });

}

void Wander::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If the agent has reached its destination, go to a random new location
    {
        Node* target = agent->GetMap()->GetRandomNode();
        agent->GoTo(target);
    }
}

// Follow
Follow::Follow(Agent* target)
	:m_target(target)
{
}

void Follow::Enter(Agent* agent)
{
    // Make agent purple when following
    agent->SetColour({ 255, 0, 255, 255 });
}

void Follow::Update(Agent* agent, float deltaTime)
{
    if (m_lastTargetNode != m_target->GetCurrentNode()) // If the target has moved, repath to them
    {
        m_lastTargetNode = m_target->GetCurrentNode();
        agent->GoTo(m_lastTargetNode);
    }
}

// MeleeAttack
MeleeAttack::MeleeAttack(Agent* target, float range)
	: m_target(target), m_range(range)
{
}

void MeleeAttack::Enter(Agent* agent)
{
    glm::vec2 distance = m_target->GetPosition() - agent->GetPosition();
    if (glm::length(distance) < agent->GetMap()->GetTileSize() * m_range) // If target is in range, attack them
    {
        std::cout << "BAM" << std::endl;

    }
}

// MeleeMove
MeleeMove::MeleeMove(Agent* target)
    :m_target(target)
{
}

void MeleeMove::Enter(Agent* agent)
{
    std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), m_target->GetCurrentNode());
    if (desiredPath.empty()) // If there's no path to the target, don't move
    {
        std::cout << "No path" << std::endl;
        return;
    }

    float maxMoveScaled = agent->GetMaxMove() * agent->GetMap()->GetTileSize();
    if (m_target->GetCurrentNode()->m_gScore > maxMoveScaled) // If the target is further away than the agent can reach
    {
        Node* furthestNode = nullptr;
        for (int i = 0; desiredPath[i]->m_gScore < maxMoveScaled; i++) // Repeat for each node in the path
        {
            furthestNode = desiredPath[i]; // If the agent can reach this node, set it as the new target
        }
        agent->GoTo(furthestNode);
    }
    else
    {
        agent->GoTo(desiredPath[desiredPath.size() - 2]); // The node just before the player
    }
}
