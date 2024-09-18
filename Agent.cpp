#include "Agent.h"

//Agent::Agent(NodeMap* map, State* state)
//	: m_state(state), m_map(map), m_colour({255, 0, 0, 255})
//{
//	PathAgent pathAgent;
//	m_pathAgent = pathAgent;
//}
//
//Agent::Agent(NodeMap* map, State* state, Color colour)
//	:m_map(map), m_state(state), m_colour(colour)
//{
//	PathAgent pathAgent;
//	m_pathAgent = pathAgent;
//}

Agent::Agent(NodeMap* map, Behaviour* behaviour)
	: m_map(map), m_behaviour(behaviour), m_colour({ 0, 0, 255, 255 }), m_maxMove(1), m_health(1), m_atk(1)
{
	PathAgent pathAgent;
	m_pathAgent = pathAgent;
}

//Agent::~Agent()
//{
//	delete m_state; // Why?
//}

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
	m_movesLeft = max; // If I end up setting max move DURING a turn this could cause issues
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

			// Update which nodes are occupied (COULD CAUSE ISSUES IF IT DOESN'T REACH THE TARGET)
			//m_pathAgent.GetCurrentNode()->m_occupant = nullptr; // Set the current node as unoccupied 
			//node->m_occupant = this; // Set self as occupying the target node
		}
	}
}

void Agent::FollowPath(std::vector<Node*> path)
{
	m_pathAgent.FollowPath(path);
	if (m_pathAgent.OnPath()) // If there was a valid path to the node
	{
		int gScoreScaled = path[path.size() - 1]->m_gScore / m_map->GetTileSize();
		m_movesLeft -= gScoreScaled; // Subtract the cost of the path from the moves left
	}
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

bool Agent::TurnComplete()
{
	return m_turnComplete;
}

bool Agent::PathComplete()
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

int Agent::GetMaxMoveScaled() const
{
	return m_maxMove * m_map->GetTileSize();
}

int Agent::GetMovesLeft() const
{
	return m_movesLeft;
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
	m_pathAgent.GetCurrentNode()->m_occupant = nullptr; // Clear self from current node

	// Update the state (if there is one)
	if (m_behaviour)
	{
		m_behaviour->Update(this, deltaTime);
	}

	Node* lastNode = m_pathAgent.GetCurrentNode();

	// Move the agent
	m_pathAgent.Update(deltaTime);

	m_pathAgent.GetCurrentNode()->m_occupant = this; // Set the current node as occupied by this agent
}

void Agent::Draw()
{
	m_pathAgent.Draw(m_colour);

	glm::vec2 pos = m_pathAgent.GetPosition();
	std::string healthText = std::to_string(m_health);
	raylib::Color::White().DrawText(healthText, pos.x - 3, pos.y - 4, 10);
}
