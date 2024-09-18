#pragma once

#include "Pathfinder.h"

class SearchParam
{
public:
	virtual float Evaluate(Node* node) = 0;
	virtual float GetCost(Edge* edge);
};

class FleeParam : public SearchParam
{
	Agent* m_target; // The agent being fled from
public:
	FleeParam(Agent* target);

	float Evaluate(Node* node) override;
	float GetCost(Edge* edge) override;
};

class LineOfSight : public SearchParam
{
	Agent* m_target; // The agent being targeted
public:
	LineOfSight(Agent* target);

	float Evaluate(Node* node) override;
};

std::vector<Node*> BestPath(Agent* agent, SearchParam* param);
