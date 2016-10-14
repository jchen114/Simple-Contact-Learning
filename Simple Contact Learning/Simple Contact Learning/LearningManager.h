#pragma once

#include "Action.h"
#include "State.h"

class LearningManager
{
public:
	LearningManager();
	~LearningManager();

	Action QueryForAction(State queryState);

};

