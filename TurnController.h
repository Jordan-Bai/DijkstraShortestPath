#pragma once

#include "Agent.h"

class TurnController
{
	Agent* m_player;
	std::vector<Agent*> m_agents;

	int m_agentIndex;
	bool m_isPlayerTurn = true;
	int m_turnCount;

	// So we only have to calculate the path to the hovered tile when it changes, instead of ever time draw is called
	Node* m_hoveredTile = nullptr;
	std::vector<Node*> m_hoveredPath;

public:
	TurnController(Agent* player);

	void AddAgent(Agent* agent);
	void RemoveAgent(Agent* agent);

	void StartPlayerTurn();
	void StartEnemyTurn();
	void EndBattle();

	void Update(float deltaTime);
	void Draw();
};
