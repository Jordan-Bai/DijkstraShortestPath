#pragma once

#include "Agent.h"

class TurnController
{
	Agent* m_player;
	std::vector<Agent*> m_agents;

	int m_agentIndex;
	bool m_isPlayerTurn = true;
	int m_turnCount;
	bool m_battleOver = false;

	// So we only have to calculate the path to the hovered tile when it changes, instead of every time draw is called
	Node* m_hoveredTile = nullptr;
	std::vector<Node*> m_hoveredPath;

public:
	TurnController(Agent* player);

	void AddAgent(Agent* agent);

	void StartPlayerTurn();
	void StartEnemyTurn();
	bool BattleOver() const;

	void Update(float deltaTime);
	void Draw();
};
