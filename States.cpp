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

// By default, don't do anything on enter or exit
void State::Enter(Agent* agent)
{
}

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
        float maxMoveScaled = agent->GetMaxMoveScaled();

        std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), target, maxMoveScaled);
        if (desiredPath.empty()) // If there's no path to the target, don't move
        {
            return;
        }

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

// Player
void Player::Update(Agent* agent, float deltaTime)
{
    if (IsMouseButtonPressed(0))
    {
        Vector2 mousePos = GetMousePosition();
        Node* target = agent->GetMap()->GetNearestNode(mousePos.x, mousePos.y);
        std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), target, agent->GetMaxMoveScaled());
        if (desiredPath.empty()) // If there's no path to the target, don't move
        {
            return;
        }

        float movesLeftScaled = agent->GetMovesLeft() * agent->GetMap()->GetTileSize();
        if (target->m_gScore <= movesLeftScaled) // If the target node is in range, go to it
        {
            agent->GoTo(target);
        }
    }
    if (IsKeyPressed(KEY_X) && agent->PathComplete()) // Only attack if the player isn't already moving
    {
        Attack(agent);
    }
}

void Player::Attack(Agent* agent)
{
    // Check each tile next to the player
    std::vector<Edge> neighbours = agent->GetCurrentNode()->m_connections;
    for (int i = 0; i < neighbours.size(); i++)
    {
        if (neighbours[i].m_target->m_occupant != nullptr) // If one of the adjacent tiles has an agent in it
        {
            neighbours[i].m_target->m_occupant->TakeDamage(agent->GetAttack()); // Attack that agent
        }
    }
    std::cout << "ATTACKING" << std::endl;
    agent->FinishTurn();
}

// MeleeChase
MeleeChase::MeleeChase(Agent* target)
	: m_target(target)
{
}

void MeleeChase::Enter(Agent* agent)
{
    agent->SetColour({ 0, 0, 255, 255 }); // Make agent blue
}

void MeleeChase::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If they're not moving
    {
        if (agent->GetMovesLeft() > 0) // Otherwise, if they can still move, do so
        {
            float maxMoveScaled = agent->GetMaxMoveScaled();
            std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), m_target->GetCurrentNode(), maxMoveScaled);

            if (desiredPath.empty()) // If there's no path to the target, don't move
            {
                std::cout << "No path" << std::endl;
                agent->StopMovement();
                return;
            }
            agent->FollowPath(desiredPath);

            // FOR TESTING
            //-----------------------------------------------------------------------------------------------------
            agent->StopMovement();
            //-----------------------------------------------------------------------------------------------------
        }
        else // If they can't do anything, finish the turn
        {
            agent->FinishTurn();
        }
    }
}


// MeleeAttack
MeleeAttack::MeleeAttack(Agent* target)
	: m_target(target)
{
}

void MeleeAttack::Enter(Agent* agent)
{
    agent->SetColour({ 255, 0, 0, 255 }); // Make agent red 
}

void MeleeAttack::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If they're not moving
    {
        m_target->TakeDamage(agent->GetAttack());
        agent->FinishTurn();
    }
}


// Ranged Chase
RangedChase::RangedChase(Agent* target)
	: m_los(target)
{
}

void RangedChase::Enter(Agent* agent)
{
    agent->SetColour({ 0, 255, 0, 255 }); // Make agent green
}

void RangedChase::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If the agent isn't moving
    {
        if (agent->GetMovesLeft() > 0) // If they still have movement left, move
        {
            std::vector<Node*> path = ClosestTarget(agent, &m_los);
            agent->FollowPath(path);

            if (agent->PathComplete()) // Means it couldn't follow that path, so end turn
            {
                agent->FinishTurn();
            }

            //agent->StopMovement(); // FOR TESTING
        }
        else // Otherwise, finish turn
        {
            agent->FinishTurn();
        }
    }
}


// Ranged Attack
RangedAttack::RangedAttack(Agent* target)
	: m_target(target), m_los(target)
{
}

void RangedAttack::Enter(Agent* agent)
{
    agent->SetColour({ 255, 0, 255, 255 }); // Make agent purple
}

void RangedAttack::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If the agent isn't moving
    {
        if (agent->GetMovesLeft() > 0) // If they still have movement left, move
        {
            std::vector<Node*> betterPath = BestTarget(agent, &m_los);
            agent->FollowPath(betterPath);
            agent->StopMovement(); // FOR TESTING
        }
        else // Otherwise, attack
        {
            std::cout << "SHOOT" << std::endl;
            m_target->TakeDamage(agent->GetAttack());
            agent->FinishTurn();
        }
    }
}


// Fleeing
Fleeing::Fleeing(Agent* target)
    : m_fleeParam(target)
{
}

void Fleeing::Enter(Agent* agent)
{
    agent->SetColour({ 255, 128, 0, 255 }); // Make agent orange when fleeing
}

void Fleeing::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If the agent isn't moving
    {
        if (agent->GetMovesLeft() > 0) // If they still have movement left, move
        {
            std::vector<Node*> path = BestTarget(agent, &m_fleeParam);
            agent->FollowPath(path);
            agent->StopMovement(); // FOR TESTING
        }
        else // Otherwise, finish turn
        {
            agent->FinishTurn();
        }
    }
}
