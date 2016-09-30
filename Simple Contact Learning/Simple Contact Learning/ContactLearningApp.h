#pragma once
#include "BulletOpenGLApplication.h"
#include <deque>
#include <random>
#include <chrono>

class GLUI;
class TactileController;
class ColliderObject;

class ContactLearningApp :
	public BulletOpenGLApplication
{
public:
	ContactLearningApp();
	~ContactLearningApp();

	ContactLearningApp(ProjectionMode mode);

	virtual void InitializePhysics() override;
	virtual void ShutdownPhysics() override;

	void LoadTextures();
	void CreateGround(const btVector3 &pos = btVector3(0,0,0));
	void CreateBodies();

	void DrawShapeCallback(btScalar *transform, const btCollisionShape *shape, const btVector3 &color);
	void DrawCallback();

	virtual void Idle() override;

	void PostTickCallback(btScalar timestep);
	void PreTickCallback(btScalar timestep);

	float GetNoise(float mean, float variance);

	GameObject* MakeBump(const float radius, const btVector3 &position, const btVector3 &color = btVector3(1.0f, 1.0f, 1.0f));
	void MakeBumps(const btVector3 &start, int numBumps, float radius = 0.3f, const btVector3 &color = btVector3(1.0f, 1.0f, 1.0f), float separation = 2.0f, float sigma = 0.0f);
	void ManageBumps();

	void SetupGUI();

	void FeelerStart();
	void FeelerPause();
	void FeelerStop();

private:

	
	std::default_random_engine m_generator;
	std::normal_distribution<double> m_distribution;

	ColliderObject *m_feeler;
	GameObject *m_ground;
	GLuint m_ground_texture;
	std::deque<GameObject *> bumps;

	int m_no_contact_pts = 15;
	float m_variance = 1.0f;
	float m_mean_separation = 7.0f;

	float SampleSeparation();
	void SetSampleParams(float mean_sep, float var);

	btVector3 m_oldPos;

	void FollowFeelerCamera();

	GLUI *m_glui_window;

	TactileController *m_controller;

};

static void ContactLearningIdle();
static void StartButtonPressed();
static void PauseButtonPressed();
static void StopButtonPressed();

void InternalPostTickCallback(btDynamicsWorld *world, btScalar timestep);
void InternalPreTickCallback(btDynamicsWorld *world, btScalar timestep);