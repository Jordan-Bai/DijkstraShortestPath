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

std::string State::GetID()
{
    return m_id; 
}


// Player
Player::Player()
{
    m_id = "Player";
}

void Player::Update(Agent* agent, float deltaTime)
{
    if (IsMouseButtonPressed(0))
    {
        Vector2 mousePos = GetMousePosition();
        Node* target = agent->GetMap()->GetNearestNode(mousePos.x, mousePos.y);

        // If the target has an occupant that isn't this agent, it's an invalid target
        if (target->m_occupant && target->m_occupant != agent)
        {
            return;
        }

        // Otherwise, pathfind to it
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
    agent->FinishTurn();
}


// MeleeChase
MeleeChase::MeleeChase(Agent* target)
	: m_target(target)
{
    m_id = "MeleeChase";
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
            agent->FollowPath(desiredPath);

            if (agent->PathComplete()) // Means it couldn't follow that path, so end turn
            {
                agent->FinishTurn();
            }
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
    m_id = "MeleeAttack";
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
	: m_losParam(target)
{
    m_id = "RangedChase";
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
            std::vector<Node*> path = ClosestTarget(agent, &m_losParam);
            agent->FollowPath(path);

            if (agent->PathComplete()) // Means it couldn't follow that path, so end turn
            {
                agent->FinishTurn();
            }
        }
        else // Otherwise, finish turn
        {
            agent->FinishTurn();
        }
    }
}


// Ranged Attack
RangedAttack::RangedAttack(Agent* target)
	: m_target(target), m_losParam(target)
{
    m_id = "RangedAttack";
}

void RangedAttack::Enter(Agent* agent)
{
    agent->SetColour({ 200, 0, 255, 255 }); // Make agent purple
}

void RangedAttack::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If the agent isn't moving
    {
        if (agent->GetMovesLeft() > 0) // If they still have movement left, move
        {
            std::vector<Node*> betterPath = BestTarget(agent, &m_losParam);
            agent->FollowPath(betterPath);
            agent->StopMovement(); // FOR TESTING
        }
        else // Otherwise, attack
        {
            m_target->TakeDamage(agent->GetAttack());
            agent->FinishTurn();
        }
    }
}


// Fleeing
Fleeing::Fleeing(Agent* agent)
    : m_fleeParam(agent)
{
    m_id = "Fleeing";
}

void Fleeing::Enter(Agent* agent)
{
    agent->SetColour({ 255, 128, 0, 255 }); // Make agent orange
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
