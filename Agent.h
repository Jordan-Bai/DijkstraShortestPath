#pragma once

#include <Color.hpp>
#include "PathAgent.h"
#include "Pathfinder.h"
#include "States.h"

class Agent
{
	PathAgent m_pathAgent;
	State* m_state;
	NodeMap* m_map;
	Color m_colour;

public:
	Agent(NodeMap* map, State* state);
	Agent(NodeMap* map, State* state, Color colour);
	~Agent();

	void SetSpeed(float speed);
	void SetNode(Node* node);

	void GoTo(int x, int y);
	void GoTo(Node* node);

	bool PathComplete();

	NodeMap* GetMap() const;
	Node* GetCurrentNode() const;

	void Update(float deltaTime);
	void Draw();
};
