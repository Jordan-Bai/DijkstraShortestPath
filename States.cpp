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

// MeleeAttack
MeleeAttack::MeleeAttack(Agent* target, float range)
	: m_target(target), m_range(range)
{
}

void MeleeAttack::Enter(Agent* agent)
{
    agent->SetColour({ 0, 0, 255, 255 }); // Make agent blue 
}

void MeleeAttack::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) // If they're not moving
    {
        glm::vec2 distance = m_target->GetPosition() - agent->GetPosition();
        if (glm::length(distance) < agent->GetMap()->GetTileSize() * m_range) // If target is in range, attack them
        {
            Attack(agent);
        }
        else if (agent->GetMovesLeft() > 0) // Otherwise, if they can still move, do so
        {
            Move(agent);
        }
        else // If they can't do anything, finish the turn
        {
            agent->FinishTurn();
        }
    }
}

void MeleeAttack::Move(Agent* agent)
{
    float maxMoveScaled = agent->GetMaxMoveScaled();
    std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), m_target->GetCurrentNode(), maxMoveScaled);

    if (desiredPath.empty()) // If there's no path to the target, don't move
    {
        std::cout << "No path" << std::endl;
        agent->StopMovement();
        return;
    }

    // Go to the furthest valid node along that path
    //--------------------------------------------------------------------------------------
    Node* furthestNode = nullptr;
    // Repeat for each node in range that doesn't have another agent already in it
    for (int i = 0; i < desiredPath.size() && desiredPath[i]->m_gScore < maxMoveScaled; i++)
    {
        // If the node is occupied, don't set it as the furthest node, but continue the loop (means agents can pass through an occupied node on their path,
        // but will never try to stop at an occupied node)
        if (desiredPath[i]->m_occupant == nullptr)
        {
            furthestNode = desiredPath[i]; // If the agent can reach this node, set it as the new target
        }
    }
    agent->GoTo(furthestNode);
    //--------------------------------------------------------------------------------------

    agent->StopMovement(); // FOR TESTING
}

void MeleeAttack::Attack(Agent* agent)
{
    glm::vec2 distance = m_target->GetPosition() - agent->GetPosition();
    if (glm::length(distance) < agent->GetMap()->GetTileSize() * m_range) // If target is in range, attack them
    {
        m_target->TakeDamage(agent->GetAttack());
    }
    agent->FinishTurn();
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
            std::vector<Node*> path = BestPath(agent, &m_los);
            agent->FollowPath(path);
            agent->StopMovement(); // FOR TESTING
        }
        else // Otherwise, finish turn
        {
            agent->FinishTurn();
        }
    }
}


// Ranged Attack
RangedAttack::RangedAttack(Agent* target)
	:m_los(target)
{
}

void RangedAttack::Enter(Agent* agent)
{
	
}

void RangedAttack::Update(Agent* agent, float deltaTime)
{
    std::cout << "SHOOT" << std::endl;
    agent->FinishTurn();
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
            std::vector<Node*> path = BestPath(agent, &m_fleeParam);
            agent->FollowPath(path);
            agent->StopMovement(); // FOR TESTING
        }
        else // Otherwise, finish turn
        {
            agent->FinishTurn();
        }
    }
}
