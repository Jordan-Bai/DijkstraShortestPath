#pragma once

#include "States.h"
#include "raylib.h"

class CloserThan : public Behaviour
{
	Agent* m_target; // The target whose distance we're finding
	float m_distance; // The distance being compared against

public:
	CloserThan(Agent* target, float distance);

	bool Execute(Agent* agent) override;
};

class LowHealth : public Behaviour
{
	int m_hpThreshold; // The HP value being compared against

public:
	LowHealth(int hpThreshold);

	bool Execute(Agent* agent) override;
};

class HasLineOfSight : public Behaviour
{
	Agent* m_target; // The target whose distance we're finding

public:
	HasLineOfSight(Agent* target);

	bool Execute(Agent* agent) override;
};

class IsMoving : public Behaviour
{
public:
	bool Execute(Agent* agent) override;
};

class CanMove : public Behaviour
{
public:
	bool Execute(Agent* agent) override;
};

class KeyPressed : public Behaviour
{
	KeyboardKey m_key;

public:
	KeyPressed(KeyboardKey key);

	bool Execute(Agent* agent) override;
};

class MousePressed : public Behaviour
{
	MouseButton m_button;

public:
	MousePressed(MouseButton button);

	bool Execute(Agent* agent) override;
};
