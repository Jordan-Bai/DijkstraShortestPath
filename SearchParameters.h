#pragma once

#include "Pathfinder.h"

class SearchParam
{
public:
	// Separate functions for Evaluate & Valid target, so we can use a search parameter to either search for the best
	// node in range, or find the closest node fitting the criteria
	virtual float Evaluate(Node* node) = 0; // Ranks how good the node is
	virtual bool ValidTarget(Node* node) = 0; // Returns whether this node fits the search criteria

	virtual float GetCost(Edge* edge);
};

class FleeParam : public SearchParam
{
	Agent* m_target; // The agent being fled from
public:
	FleeParam(Agent* target);

	float Evaluate(Node* node) override;
	bool ValidTarget(Node* node) override;

	float GetCost(Edge* edge) override;
};

class LineOfSight : public SearchParam
{
	Agent* m_target; // The agent being targeted
public:
	LineOfSight(Agent* target);
	bool ValidTarget(Node* node) override;

	float Evaluate(Node* node) override;
};

std::vector<Node*> BestTarget(Agent* agent, SearchParam* param); // Returns path to the best suited node in range

std::vector<Node*> ClosestTarget(Agent* agent, SearchParam* param); // Returns path to the closest node fitting the parameters
