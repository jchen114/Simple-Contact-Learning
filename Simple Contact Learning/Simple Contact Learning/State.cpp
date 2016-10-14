#include "stdafx.h"
#include "State.h"

#include <algorithm>

State::State(std::vector<btVector3> forces)
{
	std::for_each(forces.begin(), forces.end(), [this] (btVector3 vec){
		m_state.push_back(vec.getY()); // Only get the y component of the force...
	});

}

State::~State()
{
}

std::vector<float> State::GetContactState() {
	return m_state;
}

