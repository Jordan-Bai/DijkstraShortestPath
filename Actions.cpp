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
	agent->FinishTurn();

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
	// DO ACTION

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
	// DO ACTION

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
