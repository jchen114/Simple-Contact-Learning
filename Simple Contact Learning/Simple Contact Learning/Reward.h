#pragma once

#include "LinearMath\btVector3.h"
#include <set>

class ColliderObject;
class CollideeObject;

class Reward
{
public:
	Reward(ColliderObject *feelerObj, std::set<CollideeObject *>collisionObjs, float lowerBound);
	~Reward();

	float GetReward();

private:
	float m_reward;

	float RewardFunc(float query, float width, float lowerBound);

};

