#pragma once

#include "States.h"
#include "SearchParameters.h"

class MeleeChase : public Behaviour
{
	Agent* m_target;

public:
	MeleeChase(Agent* target);

	bool Execute(Agent* agent) override;
};

class MeleeAttack : public Behaviour
{
	Agent* m_target;
	
public:
	MeleeAttack(Agent* target);

	bool Execute(Agent* agent) override;
};

class RangedChase : public Behaviour
{
	LineOfSightParam m_losParam;

public:
	RangedChase(Agent* target);

	bool Execute(Agent* agent) override;
};

class RangedAdjust : public Behaviour
{
	LineOfSightParam m_losParam;

public:
	RangedAdjust(Agent* target);

	bool Execute(Agent* agent) override;
};

class RangedAttack : public Behaviour
{
	Agent* m_target;
	LineOfSightParam m_losParam;

public:
	RangedAttack(Agent* target);

	bool Execute(Agent* agent) override;
};

class Flee : public Behaviour
{
	FleeParam m_fleeParam;

public:
	Flee(Agent* agent);

	bool Execute(Agent* agent) override;
};

class FinishTurn : public Behaviour
{
public:
	bool Execute(Agent* agent) override;
};

class Attack : public Behaviour
{
	Agent* m_target;

public:
	Attack(Agent* target);

	bool Execute(Agent* agent) override;
};

class PlayerAttack : public Behaviour
{
	Agent* m_target;

public:
	bool Execute(Agent* agent) override;
};

class PlayerMove : public Behaviour
{
public:
	bool Execute(Agent* agent) override;
};