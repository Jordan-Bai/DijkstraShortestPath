#include "TurnController.h"

#include <iostream>

TurnController::TurnController(Agent* player)
	:m_player(player)
{
}

void TurnController::AddAgent(Agent* agent)
{
	if (agent) // If the pointer isn't null
	{
		m_agents.push_back(agent);
	}
}

void TurnController::RemoveAgent(Agent* agent)
{
	
}

void TurnController::StartPlayerTurn()
{
	m_isPlayerTurn = true;
	std::cout << "Player turn" << std::endl;
}

void TurnController::StartEnemyTurn()
{
	m_isPlayerTurn = false;
	std::cout << "Enemy turn" << std::endl;
	if (m_agents.empty()) // If there are no enemies, we can't start the enemy turn
	{
		std::cout << "Empty" << std::endl;
		return;
	}
	m_isPlayerTurn = false;
	m_agentIndex = 0;

	// Move the first enemy
	m_agents[0]->StartTurn();
}

void TurnController::FinishTurn()
{
	
}

void TurnController::EndBattle()
{
	
}

void TurnController::Update(float deltaTime)
{
	// For testing:
	//--------------------------------------------------------------------------------------
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		StartEnemyTurn();
	}
	//--------------------------------------------------------------------------------------

	if (m_isPlayerTurn)
	{
		m_player->Update(deltaTime);
		// DO PLAYER TURN LOGIC
	}
	else
	{
		m_agents[m_agentIndex]->Update(deltaTime);
		if (m_agents[m_agentIndex]->TurnComplete()) // If the agent finished its turn, move on to the next one
		{
			m_agentIndex++;
			if (m_agentIndex >= m_agents.size()) // If there are no more agents in the list, the enemy turn is over
			{
				StartPlayerTurn();
			}
			else
			{
				m_agents[m_agentIndex]->StartTurn();
			}
		}
	}
}

void TurnController::Draw()
{
	m_player->Draw();
	for (Agent* a : m_agents)
	{
		a->Draw();
	}

	if (m_isPlayerTurn)
	{
		// Get the tile the player is hovering over
		Vector2 mousePos = GetMousePosition();
		Node* hovered = m_player->GetMap()->GetNearestNode(mousePos.x, mousePos.y);
		if (hovered) // If the cell they're hovering over isn't a wall
		{
			float tileSize = m_player->GetMap()->GetTileSize();
			// Find the path to that node
			std::vector<Node*> path = PathSearch(m_player->GetCurrentNode(), hovered);
			// Draw the path
			if (!path.empty())
			{
				// Draw the path to the node
				Color tileColour = {0, 0, 0, 64 };
				for (int i = 0; i < path.size(); i++)
				{
					if (i > m_player->GetMaxMove()) // If the tile is out of the player's max move distance
					{
						tileColour = { 255, 0, 0, 64}; // Change the colour to red to show it's out of range
					}
					if (i == path.size() - 1) // Make tile actually being hovered over more opaque
					{
						tileColour.a = 128;
					}
					DrawRectangle(path[i]->m_position.x - (tileSize / 2), path[i]->m_position.y - (tileSize / 2), 
						tileSize - 1, tileSize - 1, tileColour);
				}
			}
		}
	}
}
