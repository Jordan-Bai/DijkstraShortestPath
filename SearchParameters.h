#pragma once

#include "Pathfinder.h"

class SearchParam
{
public:
	virtual float Evaluate(Node* node) = 0;
	virtual float GetCost(Edge* edge) = 0;
};

class FleeParam : public SearchParam
{
	Agent* m_target; // The agent being fled from
public:
	FleeParam(Agent* target);

	float Evaluate(Node* node) override;
	float GetCost(Edge* edge) override;
};

std::vector<Node*> BestPath(Agent* agent, SearchParam* param);
