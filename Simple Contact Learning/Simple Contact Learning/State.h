#pragma once

#include <vector>
#include "LinearMath\btVector3.h"

class State
{
public:

	State();
	State(std::vector<btVector3> forces);
	~State();

	std::vector<float> GetContactState();

private:

	std::vector<float> m_state;

};

