#pragma once

#include <Color.hpp>
#include "PathAgent.h"
#include "Pathfinder.h"
#include "States.h"

class Agent
{
	PathAgent m_pathAgent;
	Behaviour* m_behaviour; // Can be a state or an FSM
	NodeMap* m_map;
	Color m_colour;

	bool m_turnComplete;
	int m_maxMove = 2; // Max number of tiles they can move in 1 turn
	int m_movesLeft; // Amount of tiles they can move this turn

public:
	//Agent(NodeMap* map, State* state);
	//Agent(NodeMap* map, State* state, Color colour);
	Agent(NodeMap* map, Behaviour* behaviour);
	//~Agent();

	void SetSpeed(float speed);
	void SetNode(Node* node);
	void SetColour(Color colour);
	void SetMaxMove(int max);

	void SpeedUp();
	void SlowDown();

	void GoTo(int x, int y);
	void GoTo(Node* node);
	void StopMovement();

	void StartTurn();
	void FinishTurn();
	bool TurnComplete();
	bool PathComplete();

	NodeMap* GetMap() const;
	Node* GetCurrentNode() const;
	glm::vec2 GetPosition() const;
	int GetMaxMove() const;
	int GetMovesLeft() const;

	void Update(float deltaTime);
	void Draw();
};
