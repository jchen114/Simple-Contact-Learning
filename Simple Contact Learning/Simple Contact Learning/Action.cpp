#include "stdafx.h"
#include "Action.h"


Action::Action(AgentAction action)
{
	m_myAction = action;
}


Action::~Action()
{
}

AgentAction Action::GetAction() {
	return m_myAction;
}
