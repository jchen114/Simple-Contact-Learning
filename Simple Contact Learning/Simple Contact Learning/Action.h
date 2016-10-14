#pragma once

enum AgentAction{MOVE_RIGHT_1, MOVE_RIGHT_2, MOVE_RIGHT_3, MOVE_RIGHT_4};

class Action
{
public:
	Action(AgentAction action);
	~Action();

	AgentAction GetAction();

private:

	AgentAction m_myAction;

};

