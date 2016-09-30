#include "stdafx.h"
#include "TactileController.h"
#include "ColliderObject.h"
#include "GameObject.h"


TactileController::TactileController(ColliderObject *tactileObject, float distance, float timeToRise, float timeToMove)
{
	m_tactileObj = tactileObject;
	m_move_distance = distance;

	m_controller_state = STOPPED;
	m_object_state = LIFT;

	m_timeToRise = timeToRise;
	m_timeToMove = timeToMove;
	m_timeToDrop = 3.0f;

	m_resetPosition = tactileObject->m_object->GetCOMPosition();

}


TactileController::~TactileController()
{
}

#pragma region CONTROLLER

void TactileController::StateLoop() {

	if (m_controller_state == IN_MOTION) {
		switch (m_object_state)
		{
		case LIFT: {
			if (m_reset) {
				printf("LIFTING...\n");
				m_clock.reset();
				m_reset = false;
			}

			m_duration = m_clock.getTimeMilliseconds();

			if (m_duration >= m_timeToRise * 1000) {
				m_object_state = MOVE;
				m_reset = true;
				printf("X position s = %f\n", m_tactileObj->m_object->GetCOMPosition().x());
			}
			else {
				Lift();
				OrientateUpright();
			}
		}
			break;
		case MOVE: {
			if (m_reset) {
				printf("MOVING...\n");
				m_clock.reset();
				m_reset = false;
				m_initPos = m_tactileObj->m_object->GetCOMPosition();
			}
			if (m_tactileObj->m_object->GetCOMPosition().x() - m_initPos.x() >= m_move_distance) {
				m_object_state = DROP;
				m_reset = true;
				printf("X position e = %f\n", m_tactileObj->m_object->GetCOMPosition().x());
				Drop();
			}
			else {
				// Move right at constant velocity
				// Will have to apply a force on the body to keep it afloat?
				Move();
				OrientateUpright();
			}
		}
			break;
		case DROP: {

			if (m_reset) {
				printf("DROPPING...\n");
				m_clock.reset();
				m_reset = false;
			}
			m_duration = m_clock.getTimeMilliseconds();
			if (m_duration >= m_timeToDrop * 1000) {
				m_object_state = LIFT;
				m_reset = true;
			}
		}
			break;
		default:
			break;
		}
	}
}

void TactileController::Start() {
	m_controller_state = IN_MOTION;
	m_reset = true;
}

void TactileController::Pause() {
	m_controller_state = PAUSED;
	GameObject::DisableObjects(std::vector < GameObject * > {m_tactileObj->m_object});
}

void TactileController::Stop() {
	m_controller_state = STOPPED;
	
}

void TactileController::OrientateUpright() {
	float orientation = m_tactileObj->m_object->GetOrientation();
	m_tactileObj->m_object->ApplyTorque(btVector3(0, 0, m_kp * (0 - orientation) - m_kd * m_tactileObj->m_object->GetAngularVelocity()));
}

void TactileController::Lift() {
	m_tactileObj->m_object->GetRigidBody()->setLinearVelocity(btVector3(0, 1, 0) * 2 / m_timeToRise);
}

void TactileController::Move() {
	m_tactileObj->m_object->GetRigidBody()->setLinearVelocity(btVector3(1, 0, 0) * m_move_distance / m_timeToMove);
}

void TactileController::Drop() {
	GameObject::ClearForces(std::vector < GameObject * > {m_tactileObj->m_object});
	GameObject::ClearVelocities(std::vector < GameObject * > {m_tactileObj->m_object});
}

#pragma endregion CONTROLLER

#pragma region INTERFACE

void TactileController::DistanceToMove(float distanceX) {
	m_move_distance = distanceX;
}

#pragma endregion INTERFACE