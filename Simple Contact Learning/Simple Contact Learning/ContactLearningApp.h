#pragma once
#include "BulletOpenGLApplication.h"

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
	void CreateGround();
	void CreateBodies();

	void DrawShapeCallback(btScalar *transform, const btCollisionShape *shape, const btVector3 &color);
	void DrawCallback();

	void PostTickCallback(btScalar timestep);
	void PreTickCallback(btScalar timestep);

	void MakeBump(const float radius, const btVector3 &position, const btVector3 &color = btVector3(1.0f, 1.0f, 1.0f));

private:
	GameObject *m_ground;
	GLuint m_ground_texture;

};

void InternalPostTickCallback(btDynamicsWorld *world, btScalar timestep);
void InternalPreTickCallback(btDynamicsWorld *world, btScalar timestep);