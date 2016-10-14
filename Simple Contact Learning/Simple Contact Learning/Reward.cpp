#include "stdafx.h"
#include <math.h>

#include "Reward.h"

#include "ColliderObject.h"
#include "CollideeObject.h"

Reward::Reward(ColliderObject *feelerObj, std::set<CollideeObject *>collisionObjs, float lowerBound)
{
	// Calculate the reward based on the state
	
	
}


Reward::~Reward()
{

}

float Reward::GetReward() {

	return m_reward;

}

float Reward::RewardFunc(float query, float width, float lowerBound) {
	
	if (query < width || query > width) {
		return lowerBound;
	}

	return -(lowerBound / powf(width, 2) * powf(query, 2));

}