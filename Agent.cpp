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
	: m_map(map), m_behaviour(behaviour), m_colour({ 0, 0, 255, 255 })
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
	m_pathAgent.SetCurrentNode(node);
}

void Agent::SetColour(Color colour)
{
	m_colour = colour;
}

void Agent::SetMaxMove(int max)
{
	m_maxMove = max;
}

void Agent::GoTo(int x, int y)
{
	Node* target = m_map->GetNearestNode(x, y);
	if (target)
	{
		m_pathAgent.GoToNode(target);
	}
}

void Agent::GoTo(Node* node)
{
	if (node)
	{
		m_pathAgent.GoToNode(node);
	}
}

void Agent::StartTurn()
{
	if (m_behaviour) // Only start the turn if they actually HAVE a behaviour
	{
		m_turnComplete = false; // Since it's a new turn, reset the bool
		m_behaviour->Move(this);
	}
}

bool Agent::TurnComplete()
{
	return m_turnComplete;
}

bool Agent::PathComplete()
{
	return m_pathAgent.OnPath();
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

void Agent::Update(float deltaTime)
{
	// Check inputs
	if (IsKeyPressed(KEY_W))
	{
		m_pathAgent.SpeedUp();
	}
	if (IsKeyPressed(KEY_S))
	{
		m_pathAgent.SlowDown();
	}

	// Update the state (if there is one)
	if (m_behaviour)
	{
		m_behaviour->Update(this, deltaTime);
	}

	// Move the agent
	m_pathAgent.Update(deltaTime);

	if (!m_pathAgent.OnPath() && m_behaviour) // If they're not on a path (no longer moving) & have a behaviour
	{
		m_behaviour->Action(this);
		m_turnComplete = true;
	}
}

void Agent::Draw()
{
	m_pathAgent.Draw(m_colour);
}
