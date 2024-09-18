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
	std::cout << std::endl << "Player turn" << std::endl;
	m_player->StartTurn();
}

void TurnController::StartEnemyTurn()
{
	std::cout << std::endl << "Enemy turn" << std::endl;
	if (m_agents.empty()) // If there are no enemies, we can't start the enemy turn
	{
		std::cout << "Empty" << std::endl;
		StartPlayerTurn();
		return;
	}
	m_isPlayerTurn = false;
	m_agentIndex = 0;

	// Move the first enemy
	m_agents[0]->StartTurn();
}

void TurnController::EndBattle()
{
	
}

void TurnController::Update(float deltaTime)
{

	if (m_isPlayerTurn)
	{
		m_player->Update(deltaTime);
		// DO PLAYER TURN LOGIC
		
		// Player inputs
		//--------------------------------------------------------------------------------------
		if ((IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) 
			&& m_player->PathComplete()) // Player isn't still moving
		{
			StartEnemyTurn(); // End the player's turn
		}
		//--------------------------------------------------------------------------------------

		if (m_player->TurnComplete())
		{
			StartEnemyTurn();
		}
	}
	else
	{
		if (m_agents[m_agentIndex]->IsDead()) // If the agent is dead, remove it from the list of agents
		{
			m_agents.erase(m_agents.begin() + m_agentIndex);

			if (m_agentIndex >= m_agents.size()) // If the dead agent was the last one in the list, end the enemy turn
			{
				StartPlayerTurn();
				return;
			}

			// Otherwise, start the next agent's turn
			m_agents[m_agentIndex]->StartTurn();
		}

		m_agents[m_agentIndex]->Update(deltaTime);

		if (m_agents[m_agentIndex]->TurnComplete()) // If the agent finished its turn, move on to the next one
		{
			m_agentIndex++;
			// FOR TESTING
			//-----------------------------------------------------------------------------------------------------
			if (m_agentIndex == 1)
			{
				std::cout << "a" << std::endl;
			}
			//-----------------------------------------------------------------------------------------------------
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

	if (IsKeyPressed(KEY_W))
	{
		m_player->SpeedUp();
		for (int i = 0; i < m_agents.size(); i++)
		{
			m_agents[i]->SpeedUp();
		}
	}
	if (IsKeyPressed(KEY_S))
	{
		m_player->SlowDown();
		for (int i = 0; i < m_agents.size(); i++)
		{
			m_agents[i]->SlowDown();
		}
	}
}

void TurnController::Draw()
{
	m_player->Draw();
	for (Agent* a : m_agents)
	{
		if (!a->IsDead()) // If the agent isn't dead (just in case agent dies but hasn't been removed from the list yet)
		{
			a->Draw();
		}
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
				float movesLeftScaled = m_player->GetMovesLeft() * m_player->GetMap()->GetTileSize(); // For checking if the tile is in range
				for (int i = 0; i < path.size(); i++)
				{
					if (path[i]->m_gScore > movesLeftScaled) // If the tile is out of the player's max move distance
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
