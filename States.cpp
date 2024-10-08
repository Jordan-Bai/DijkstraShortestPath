#include "States.h"

#include <iostream>
#include "Agent.h"

void Composite::AddChild(Behaviour* behaviour)
{
	m_children.push_back(behaviour);
}

bool Selector::Execute(Agent* agent)
{
	for (Behaviour* b : m_children)
	{
		if (b->Execute(agent)) // if the behaviour returns true, return without going through the other behaviours
		{
			return true;
		}
	}
	return false; // If none of the children returned true, return false
}

bool Sequence::Execute(Agent* agent)
{
	for (Behaviour* b : m_children)
	{
		if (!b->Execute(agent)) // if the behaviour returns false, return without going through the other behaviours
		{
			return false;
		}
	}
	return true; // If none of the children returned false, return true
}
