#pragma once

#include "Pathfinder.h"

class SearchParam // Abstract base class for Search parameters
{
protected:
	Agent* m_target;
public:
	// Separate functions for Evaluate & Valid target, so we can use a search parameter to either search for the best
	// node in range, or find the closest node fitting the criteria
	virtual float Evaluate(Node* node) = 0; // Ranks how good the node is
	virtual bool ValidTarget(Node* node) = 0; // Returns whether this node fits the search criteria

	virtual float GetHScore(Edge* edge);
};

class FleeParam : public SearchParam
{
public:
	FleeParam(Agent* target);

	float Evaluate(Node* node) override;
	bool ValidTarget(Node* node) override;

	float GetHScore(Edge* edge) override;
};

class LineOfSightParam : public SearchParam
{
public:
	LineOfSightParam(Agent* target);

	float Evaluate(Node* node) override;
	bool ValidTarget(Node* node) override;
};

std::vector<Node*> BestTarget(Agent* agent, SearchParam* param); // Returns path to the best suited node in range

std::vector<Node*> ClosestTarget(Agent* agent, SearchParam* param); // Returns path to the closest node fitting the parameters
