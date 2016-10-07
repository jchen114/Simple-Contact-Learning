#pragma once

#include "LinearMath\btQuickprof.h"
#include "LinearMath\btVector3.h"

class GameObject;
class ColliderObject;

enum CurrentObjectState {
	LIFT, 
	MOVE,
	DROP
};

enum CurrentControllerState {
	STOPPED,
	IN_MOTION,
	PAUSED
};

class TactileController
{
public:
	TactileController(ColliderObject *tactileObject, float distance = 1.0f, float timeToRise = 1.0f, float timeToMove = 2.0f);

	void DistanceToMove(float distanceX);

	void StateLoop();
	
	void Stop();
	void Pause();
	void Start();

	void GatherState();

	~TactileController();

private:

	btVector3 m_resetPosition;

	ColliderObject *m_tactileObj;
	float m_move_distance;

	float m_timeToRise;
	float m_timeToMove;
	float m_timeToDrop;

	CurrentObjectState m_object_state;
	CurrentControllerState m_controller_state;

	void OrientateUpright();
	void Lift();
	void Move();
	void Drop();

	btClock m_clock;
	bool m_reset = false;
	unsigned long m_duration;

	float m_kp = 30.0f;
	float m_kd = 3.0f;

	btVector3 m_initPos;

};

