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

void TurnController::StartPlayerTurn()
{
	if (m_player->IsDead()) // Means the battle is over
	{
		m_battleOver = true;
		return;
	}

	m_isPlayerTurn = true;
	m_player->StartTurn();

	// Recalculate the path to the hovered tile so it's drawn correctly
	Vector2 mousePos = GetMousePosition();
	Node* hovered = m_player->GetMap()->GetNearestNode(mousePos.x, mousePos.y);
	m_hoveredTile = hovered;
	if (!hovered) // If hovered is null, don't do a path search, just make the path empty
	{
		m_hoveredPath = {};
	}
	else
	{
		m_hoveredPath = PathSearch(m_player->GetCurrentNode(), hovered, m_player->GetMaxMoveScaled());
	}
}

void TurnController::StartEnemyTurn()
{
	if (m_agents.empty()) // If there are no enemies, the battle is over & we can't start the enemy turn
	{
		m_battleOver = true;
		StartPlayerTurn();
		return;
	}
	m_isPlayerTurn = false;
	m_agentIndex = 0;

	// Move the first enemy
	m_agents[0]->StartTurn();
}

bool TurnController::BattleOver() const
{
	return m_battleOver;
}

void TurnController::Update(float deltaTime)
{
	if (!m_battleOver) // Only update if the battle is still going
	{
		if (m_agents.empty()) // If there are no enemies, the battle is over 
		{
			m_battleOver = true;
			return;
		}

		if (m_isPlayerTurn)
		{
			m_player->Update(deltaTime);

			// Inputs for ending player turn
			if ((IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
				&& m_player->PathComplete()) // Player isn't still moving
			{
				StartEnemyTurn(); // End the player's turn
			}

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
			for (Agent* a : m_agents)
			{
				a->SpeedUp();
			}
		}
		if (IsKeyPressed(KEY_S))
		{
			m_player->SlowDown();
			for (Agent* a : m_agents)
			{
				a->SlowDown();
			}
		}
	}
}

void TurnController::Draw()
{
	// Draw the path to the tile the player is hovering over
	if (m_isPlayerTurn && m_player->PathComplete() && !m_battleOver) // If it's the player's turn, they're not moving & the battle's still going
	{
		// Get the tile the player is hovering over
		Vector2 mousePos = GetMousePosition();
		Node* hovered = m_player->GetMap()->GetNearestNode(mousePos.x, mousePos.y);
		if (hovered != m_hoveredTile) // If they're hovering over a different tile, recalculate the path
		{
			// Find the path to that node
			m_hoveredTile = hovered;
			if (!hovered) // If hovered is null, don't do a path search, just make the path empty
			{
				m_hoveredPath = {};
			}
			else
			{
				m_hoveredPath = PathSearch(m_player->GetCurrentNode(), hovered, m_player->GetMaxMoveScaled());
			}
		}

		if (m_hoveredTile) // If the cell they're hovering over isn't a wall
		{
			float tileSize = m_player->GetMap()->GetTileSize();
			// Draw the path (if it exists)
			if (!m_hoveredPath.empty())
			{
				// Draw the path to the node
				Color tileColour = { 0, 0, 0, 64 };
				float movesLeftScaled = m_player->GetMovesLeftScaled(); // For checking if the tile is in range
				for (int i = 0; i < m_hoveredPath.size(); i++)
				{
					if (m_hoveredPath[i]->m_gScore > movesLeftScaled) // If the tile is out of the player's max move distance
					{
						tileColour = { 255, 0, 0, 64 }; // Change the colour to red to show it's out of range
					}
					if (i == m_hoveredPath.size() - 1) // Make tile actually being hovered over more opaque
					{
						tileColour.a = 128;
						// If the tile being hovered over has an occupant that isn't the player, it's an invalid target
						if (m_hoveredPath[i]->m_occupant && m_hoveredPath[i]->m_occupant != m_player)
						{
							tileColour.r = 255; // Change the colour to red to show it's invalid
						}
					}
					DrawRectangle(m_hoveredPath[i]->m_position.x - (tileSize / 2), m_hoveredPath[i]->m_position.y - (tileSize / 2),
						tileSize - 1, tileSize - 1, tileColour);
				}
			}
			else // If there's no path to the hovered tile, just draw it in red to show it's not reachable
			{
				DrawRectangle(m_hoveredTile->m_position.x - (tileSize / 2), m_hoveredTile->m_position.y - (tileSize / 2),
					tileSize - 1, tileSize - 1, { 255, 0, 0, 128 });
			}
		}
		else // Just draw the tile the player is on
		{
			float tileSize = m_player->GetMap()->GetTileSize();
			glm::vec2 pos = m_player->GetPosition();

			DrawRectangle(pos.x - (tileSize / 2), pos.y - (tileSize / 2),
				tileSize - 1, tileSize - 1, { 0, 0, 0, 128 });
		}
	}

	// Draw all the actual agents
	m_player->Draw();
	for (Agent* a : m_agents)
	{
		a->Draw();
	}

	if (!m_battleOver)
	{
		// Show whose turn it is
		if (m_isPlayerTurn)
		{
			DrawRectangle(0, 0, 128, 28, BLACK);
			DrawRectangle(2, 2, 124, 24, WHITE);
			DrawText("Player turn", 4, 4, 20, BLACK);
		}
		else
		{
			DrawRectangle(0, 0, 128, 28, BLACK);
			DrawRectangle(2, 2, 124, 24, RED);
			DrawText("Enemy turn", 4, 4, 20, BLACK);
		}
	}
}
