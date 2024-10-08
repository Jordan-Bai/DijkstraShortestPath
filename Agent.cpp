#include "Agent.h"

Agent::Agent(NodeMap* map, Behaviour* behaviour)
	: m_map(map), m_behaviour(behaviour), m_colour({ 0, 0, 255, 255 }), m_maxMove(1), m_health(1), m_atk(1)
{
	PathAgent pathAgent;
	m_pathAgent = pathAgent;
}

void Agent::SetSpeed(float speed)
{
	m_pathAgent.SetSpeed(speed);
}

void Agent::SetNode(Node* node)
{
	if (m_pathAgent.GetCurrentNode()) // If the agent was already at a node
	{
		m_pathAgent.GetCurrentNode()->m_occupant = nullptr; // Clear self from old node
	}
	m_pathAgent.SetCurrentNode(node);
	node->m_occupant = this;
}

void Agent::SetColour(Color colour)
{
	m_colour = colour;
}

void Agent::SetMaxMove(int max)
{
	m_maxMove = max;
	m_movesLeft = max;
}

void Agent::SetHealth(int health)
{
	m_health = health;
}

void Agent::SetAttack(int attackDamage)
{
	m_atk = attackDamage;
}


void Agent::SpeedUp()
{
	m_pathAgent.SpeedUp();
}

void Agent::SlowDown()
{
	m_pathAgent.SlowDown();
}


void Agent::GoTo(int x, int y)
{
	Node* target = m_map->GetNearestNode(x, y);
	if (target)
	{
		GoTo(target);
	}
}

void Agent::GoTo(Node* node)
{
	if (node)
	{
		float maxMoveScaled = m_maxMove * m_map->GetTileSize();
		m_pathAgent.GoToNode(node, maxMoveScaled);
		if (m_pathAgent.OnPath()) // If there was a valid path to the node
		{
			int gScoreScaled = node->m_gScore / m_map->GetTileSize();
			m_movesLeft -= gScoreScaled; // Subtract the cost of the path from the moves left

			m_pathAgent.GetCurrentNode()->m_occupant = nullptr; // Clear self from current node
			node->m_occupant = this; // Set the target node as occupied by this agent
		}
	}
}

void Agent::FollowPath(std::vector<Node*> path)
{
	if (path.empty()) // If the path is empty, ignore it
	{
		StopMovement(); // To stop agent from repeatedly being asked to follow an empty path
		return;
	}

	m_pathAgent.GetCurrentNode()->m_occupant = nullptr; // Clear self from current node

	// Go to the furthest valid node along that path
	Node* furthestNode = nullptr;
	int furthestIndex = 0;

	// Repeat for each node in range that doesn't have another agent already in it
	for (int i = 0; i < path.size() && path[i]->m_gScore <= (m_movesLeft * m_map->GetTileSize()); i++)
	{
		// If the node is occupied, don't set it as the furthest node, but continue the loop (means agents can pass through an occupied node on their path,
		// but will never try to stop at an occupied node)
		if (path[i]->m_occupant == nullptr)
		{
			furthestNode = path[i]; // If the agent can reach this node, set it as the new target
			furthestIndex = i;
		}
	}

	path.erase(path.begin() + furthestIndex + 1, path.end()); // Remove the invalid nodes from the list
	if (path.size() <= 1) // If there are no nodes/ only the starting node, return
	{
		m_pathAgent.GetCurrentNode()->m_occupant = this;
		StopMovement(); // To stop agent from repeatedly being asked to follow an impossible path
		return;
	}

	// Adjust the number of moves left
	int gScoreScaled = furthestNode->m_gScore / m_map->GetTileSize(); // Get the gScore of the last node
	m_movesLeft -= gScoreScaled; // Subtract the cost of the path from the moves left

	m_pathAgent.FollowPath(path);

	furthestNode->m_occupant = this; // Set the target node as occupied by this agent
}

void Agent::StopMovement()
{
	m_movesLeft = 0;
}


void Agent::StartTurn()
{
	if (m_behaviour) // Only start the turn if they actually HAVE a behaviour
	{
		m_turnComplete = false; // Since it's a new turn, reset the bool
		m_movesLeft = m_maxMove; // Reset the number of tiles they can move
	}
}

void Agent::FinishTurn()
{
	if (m_behaviour) // Only end the turn if they actually HAVE a behaviour
	{
		m_turnComplete = true;
	}
}

bool Agent::TurnComplete() const
{
	return m_turnComplete;
}

bool Agent::PathComplete() const
{
	return !m_pathAgent.OnPath();
}


void Agent::TakeDamage(int damage)
{
	m_health -= damage;
	if (m_health <= 0) // If this agent has died
	{
		m_pathAgent.GetCurrentNode()->m_occupant = nullptr; // Clear self from the node
	}
}

bool Agent::IsDead()
{
	return (m_health <= 0);
}


NodeMap* Agent::GetMap() const
{
	return m_map;
}

Node* Agent::GetCurrentNode() const
{
	return m_pathAgent.GetCurrentNode();
}

glm::vec2 Agent::GetPosition() const
{
	return m_pathAgent.GetPosition();
}


int Agent::GetMaxMove() const
{
	return m_maxMove;
}

int Agent::GetMaxMoveScaled() const
{
	return m_maxMove * m_map->GetTileSize();
}

int Agent::GetMovesLeft() const
{
	return m_movesLeft;
}

int Agent::GetMovesLeftScaled() const
{
	return m_movesLeft * m_map->GetTileSize();
}


int Agent::GetHealth() const
{
	return m_health;
}

int Agent::GetAttack() const
{
	return m_atk;
}


void Agent::Update(float deltaTime)
{
	// Update the state (if there is one)
	if (m_behaviour)
	{
		m_behaviour->Execute(this);
	}

	// Move the agent
	m_pathAgent.Update(deltaTime);
}

void Agent::Draw()
{
	if (m_health > 0) // Only draw the agent if they're alive
	{
		m_pathAgent.Draw(m_colour);

		glm::vec2 pos = m_pathAgent.GetPosition();
		std::string healthText = std::to_string(m_health);
		raylib::Color::White().DrawText(healthText, pos.x - 3, pos.y - 4, 10);
	}
}
