#include "Actions.h"

#include "Agent.h"

// MeleeChase
MeleeChase::MeleeChase(Agent* target)
	: m_target(target)
{
}

bool MeleeChase::Execute(Agent* agent)
{
	float maxMoveScaled = agent->GetMaxMoveScaled();
	std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), m_target->GetCurrentNode(), maxMoveScaled);
	agent->FollowPath(desiredPath);

	return true;
}

//void MeleeChase::Update(Agent* agent, float deltaTime)
//{
//    if (agent->PathComplete()) // If they're not moving
//    {
//        if (agent->GetMovesLeft() > 0) // Otherwise, if they can still move, do so
//        {
//            float maxMoveScaled = agent->GetMaxMoveScaled();
//            std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), m_target->GetCurrentNode(), maxMoveScaled);
//            agent->FollowPath(desiredPath);
//
//            if (agent->PathComplete()) // Means it couldn't follow that path, so end turn
//            {
//                agent->FinishTurn();
//            }
//        }
//        else // If they can't do anything, finish the turn
//        {
//            agent->FinishTurn();
//        }
//    }
//}


// MeleeAttack
MeleeAttack::MeleeAttack(Agent* target)
	: m_target(target)
{
}

bool MeleeAttack::Execute(Agent* agent)
{
	m_target->TakeDamage(agent->GetAttack());

	return true;
}

//void MeleeAttack::Update(Agent* agent, float deltaTime)
//{
//    if (agent->PathComplete()) // If they're not moving
//    {
//        m_target->TakeDamage(agent->GetAttack());
//        agent->FinishTurn();
//    }
//}


// Ranged Chase
RangedChase::RangedChase(Agent* target)
	: m_losParam(target)
{
}

bool RangedChase::Execute(Agent* agent)
{
	std::vector<Node*> path = ClosestTarget(agent, &m_losParam);
	agent->FollowPath(path);
	
	if (agent->PathComplete()) // Means it couldn't follow that path, so stop moving
	{
		agent->StopMovement();
	}

	return true;
}

//void RangedChase::Update(Agent* agent, float deltaTime)
//{
//    if (agent->PathComplete()) // If the agent isn't moving
//    {
//        if (agent->GetMovesLeft() > 0) // If they still have movement left, move
//        {
//            std::vector<Node*> path = ClosestTarget(agent, &m_losParam);
//            agent->FollowPath(path);
//
//            if (agent->PathComplete()) // Means it couldn't follow that path, so end turn
//            {
//                agent->FinishTurn();
//            }
//        }
//        else // Otherwise, finish turn
//        {
//            agent->FinishTurn();
//        }
//    }
//}


// Ranged Adjust
RangedAdjust::RangedAdjust(Agent* target)
	:m_losParam(target)
{
}

bool RangedAdjust::Execute(Agent* agent)
{
	std::vector<Node*> betterPath = BestTarget(agent, &m_losParam);
	if (betterPath.empty())
	{
		agent->StopMovement();
	}
	else
	{
		agent->FollowPath(betterPath);
	}

	return true;
}



// Ranged Attack
RangedAttack::RangedAttack(Agent* target)
	: m_target(target), m_losParam(target)
{
}

bool RangedAttack::Execute(Agent* agent)
{
	// DO ACTION

	return true;
}

//void RangedAttack::Update(Agent* agent, float deltaTime)
//{
//    if (agent->PathComplete()) // If the agent isn't moving
//    {
//        if (agent->GetMovesLeft() > 0) // If they still have movement left, move
//        {
//            std::vector<Node*> betterPath = BestTarget(agent, &m_losParam);
//            agent->FollowPath(betterPath);
//            agent->StopMovement(); // FOR TESTING
//        }
//        else // Otherwise, attack
//        {
//            m_target->TakeDamage(agent->GetAttack());
//            agent->FinishTurn();
//        }
//    }
//}


// Fleeing
Flee::Flee(Agent* agent)
    : m_fleeParam(agent)
{
}

bool Flee::Execute(Agent* agent)
{
	std::vector<Node*> path = BestTarget(agent, &m_fleeParam);
	if (path.empty())
	{
		agent->StopMovement();
	}
	else
	{
		agent->FollowPath(path);
	}

	return true;
}

//void Fleeing::Update(Agent* agent, float deltaTime)
//{
//    if (agent->PathComplete()) // If the agent isn't moving
//    {
//        if (agent->GetMovesLeft() > 0) // If they still have movement left, move
//        {
//            std::vector<Node*> path = BestTarget(agent, &m_fleeParam);
//            agent->FollowPath(path);
//            agent->StopMovement(); // FOR TESTING
//        }
//        else // Otherwise, finish turn
//        {
//            agent->FinishTurn();
//        }
//    }
//}

// Finish turn
bool FinishTurn::Execute(Agent* agent)
{
	agent->FinishTurn();
	return true;
}


// Attack
Attack::Attack(Agent* target)
	: m_target(target)
{
}

bool Attack::Execute(Agent* agent)
{
	m_target->TakeDamage(agent->GetAttack());

	return true;
}


// Player Attack
bool PlayerAttack::Execute(Agent* agent)
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

	return true;
}


// Player Move
bool PlayerMove::Execute(Agent* agent)
{
	Vector2 mousePos = GetMousePosition();
	Node* target = agent->GetMap()->GetNearestNode(mousePos.x, mousePos.y);
	
	// If the target is a nullpointer, return
	if (!target)
	{
		return true; // As this is an action, it always returns true
	}
	// If the target has an occupant that isn't this agent, it's an invalid target
	else if (target->m_occupant && target->m_occupant != agent)
	{
	    return true;
	}
	
	// Otherwise, pathfind to it
	std::vector<Node*> desiredPath = PathSearch(agent->GetCurrentNode(), target, agent->GetMaxMoveScaled());
	if (desiredPath.empty()) // If there's no path to the target, don't move
	{
	    return true;
	}
	
	float movesLeftScaled = agent->GetMovesLeft() * agent->GetMap()->GetTileSize();
	if (target->m_gScore <= movesLeftScaled) // If the target node is in range, go to it
	{
	    agent->GoTo(target);
	}

	return true;
}
