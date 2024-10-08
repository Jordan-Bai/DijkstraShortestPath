#pragma once

#include "Pathfinder.h"


class Behaviour // Abstract base class for behaviour trees
{
public:
	virtual bool Execute(Agent* agent) = 0;
};

class Composite : public Behaviour // Abstract base class for composite behaviours
{
protected:
	std::vector<Behaviour*> m_children;

public:
	virtual bool Execute(Agent* agent) = 0;
	void AddChild(Behaviour* behaviour);
};

class Selector : public Composite // Acts as an "Or" branch
{
public:
	virtual bool Execute(Agent* agent) override;
};

class Sequence : public Composite // Acts as an "And" branch
{
public:
	virtual bool Execute(Agent* agent) override;
};
