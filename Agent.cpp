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
	: m_map(map), m_behaviour(behaviour)
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
}

void Agent::Draw()
{
	m_pathAgent.Draw(m_colour);
}
