#pragma once

#include "Agent.h"

class TurnController
{
	Agent* m_player;
	std::vector<Agent*> m_agents;

	int m_currentAgentIndex;
	bool m_isPlayerTurn = true;
	int m_turnCount;

public:
	TurnController(Agent* player);

	void AddAgent(Agent* agent);
	void RemoveAgent(Agent* agent);

	void FinishTurn();
	void EndBattle();

	void Update();
	void Draw();
};
