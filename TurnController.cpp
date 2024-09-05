#include "TurnController.h"

TurnController::TurnController(Agent* player)
	:m_player(player)
{
}

void TurnController::AddAgent(Agent* agent)
{
	
}

void TurnController::RemoveAgent(Agent* agent)
{
	
}

void TurnController::FinishTurn()
{
	
}

void TurnController::EndBattle()
{
	
}

void TurnController::Update()
{

}

void TurnController::Draw()
{
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
				for (int i = 0; i < path.size() - 1; i++)
				{
					DrawRectangle(path[i]->m_position.x - (tileSize / 2), path[i]->m_position.y - (tileSize / 2), 
						tileSize - 1, tileSize - 1, { 100, 100, 100, 100 });
				}
			}

			DrawRectangle(hovered->m_position.x - (tileSize / 2), hovered->m_position.y - (tileSize / 2),
				tileSize - 1, tileSize - 1, { 100, 100, 100, 100 });
		}
	}
}
