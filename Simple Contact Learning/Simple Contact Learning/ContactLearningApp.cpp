#include "stdafx.h"
#include "ContactLearningApp.h"
#include <SOIL\src\SOIL.h>
#include "ContactManager.h"
#include <glui\glui.h>
#include "TactileController.h"


using namespace std::placeholders;

enum Body { Ground = 0 };

static ContactLearningApp *m_app;

#pragma region INIT

ContactLearningApp::ContactLearningApp()
{
}


ContactLearningApp::~ContactLearningApp()
{
}

ContactLearningApp::ContactLearningApp(ProjectionMode mode):BulletOpenGLApplication(mode, false, btVector3(0, 0, 0), 1.0f, 30.0f, 0.0f, 0.0f, -2.0f)
{
	m_app = this;
	m_DrawShapeCallback = std::bind(&ContactLearningApp::DrawShapeCallback, this, _1, _2, _3);
	m_DrawCallback = std::bind(&ContactLearningApp::DrawCallback, this);

}

void ContactLearningApp::ShutdownPhysics() {

	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;

}

void ContactLearningApp::InitializePhysics() {

	// create the collision configuration
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	// create the dispatcher
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);

	// Adding for 2D collisions and solving
	m_pDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE, BOX_2D_SHAPE_PROXYTYPE, new btBox2dBox2dCollisionAlgorithm::CreateFunc());

	// create the broadphase
	m_pBroadphase = new btDbvtBroadphase();
	// create the constraint solver
	m_pSolver = new btSequentialImpulseConstraintSolver();
	// create the world
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);

	m_pWorld->setInternalTickCallback(InternalPostTickCallback, 0, false);
	m_pWorld->setInternalTickCallback(InternalPreTickCallback, 0, true);

	m_pWorld->getPairCache()->setOverlapFilterCallback(ContactManager::GetInstance().GetFilterCallback());

	LoadTextures();

	CreateGround(btVector3(200, 0, 0));

	CreateBodies();

	SetupGUI();

	m_controller = new TactileController(m_feeler,m_mean_separation);

	SetSampleParams(m_mean_separation, m_variance);

	MakeBumps(
		btVector3(m_feeler->m_object->GetCOMPosition().x() + SampleSeparation(), 0.4, 0.0f), 
		5, 
		0.22f, 
		btVector3(0.0f, 1.0f, 0.0f), 
		m_mean_separation, 
		m_variance);

	// Test bump
	//MakeBump(0.3, btVector3(0, 0.5, 0), btVector3(0, 1, 0));

}

void ContactLearningApp::Idle() {
	BulletOpenGLApplication::Idle();
}

void ContactLearningApp::SetupGUI() {

	GLUI_Master.set_glutIdleFunc(ContactLearningIdle);

	m_glui_window = GLUI_Master.create_glui("Contact Learning Controls");
	GLUI_Panel *parameters_panel = m_glui_window->add_panel("Parameters");
	m_glui_window->add_spinner_to_panel(parameters_panel, "Contact Points", GLUI_SPINNER_INT, &m_no_contact_pts);
	m_glui_window->add_spinner_to_panel(parameters_panel, "Mean", GLUI_SPINNER_FLOAT, &m_mean_separation);
	m_glui_window->add_spinner_to_panel(parameters_panel, "Variance", GLUI_SPINNER_FLOAT, &m_variance);

	m_glui_window->add_column(true);

	GLUI_Panel *controls_panel = m_glui_window->add_panel("Controls");
	m_glui_window->add_button_to_panel(controls_panel, "Start", NULL, (GLUI_Update_CB) StartButtonPressed);
	m_glui_window->add_button_to_panel(controls_panel, "Pause", NULL, (GLUI_Update_CB) PauseButtonPressed);
	m_glui_window->add_button_to_panel(controls_panel, "Stop", NULL, (GLUI_Update_CB) StopButtonPressed);

	m_glui_window->set_main_gfx_window(m_main_window_id);

}

void ContactLearningApp::LoadTextures() {
	// Load up the textures
	m_ground_texture = SOIL_load_OGL_texture
		(
		"..\\..\\Dependencies\\Resources\\checkerboard.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	if (0 == m_ground_texture)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

}

void ContactLearningApp::CreateGround(const btVector3 &pos) {

	// Create 3D ground.
	m_ground = CreateGameObject(new btBoxShape(btVector3(200, 0.5f, 15)), 0, btVector3(1.0f, 1.0f, 1.0f), pos);
	ContactManager::GetInstance().AddObjectToCollideWith(m_ground);

}

void ContactLearningApp::CreateBodies() {

	// Test Collision sets
	GameObject *feeler = CreateGameObject(new btBox2dShape(btVector3(1.5f, 0.5f, 0.0f)), 0.4f, btVector3(1.0f, 1.0f, 0.0f), btVector3(5.0f, 2.0f, 0.0f));
	m_oldPos = btVector3(0, 0, 0);
	m_feeler = ContactManager::GetInstance().AddObjectForCollision(feeler, 25);

}


#pragma endregion INIT

#pragma region INTERFACE 

void ContactLearningApp::FeelerStart() {
	m_controller->Start();
}

void ContactLearningApp::FeelerPause() {
	m_controller->Pause();
}

void ContactLearningApp::FeelerStop() {
	m_controller->Stop();
}

#pragma endregion INTERFACE

#pragma region UTILS

GameObject* ContactLearningApp::MakeBump(const float radius, const btVector3 &position, const btVector3 &color) {

	GameObject *bump = CreateGameObject(new btSphereShape(radius), 0.0f, color, position);
	ContactManager::GetInstance().AddObjectToCollideWith(bump);
	return bump;

}

void ContactLearningApp::MakeBumps(const btVector3 &start, int numBumps, float radius, const btVector3 &color, float separation, float sigma) {

	for (int bump = 0; bump < numBumps; bump++) {
		btVector3 bumpPos = start + btVector3(bump * separation, 0, 0);
		bumps.push_back(MakeBump(radius, bumpPos, color));
	}

}

void ContactLearningApp::ManageBumps() {

	// Get the middle bump..
	GameObject *bump = bumps.at(ceil(bumps.size() / 2));

	// Check if feeler is past the bump..
	if (m_feeler->m_object->GetCOMPosition().x() >= bump->GetCOMPosition().x()) {
		printf("Recycle Bump\n");
		GameObject *firstBump = bumps.front();
		GameObject *lastBump = bumps.back();
		firstBump->Reposition(btVector3(lastBump->GetCOMPosition().x() + SampleSeparation(), lastBump->GetCOMPosition().y(), 0.0f));
		bumps.pop_front();
		bumps.push_back(firstBump);
	}

}


float ContactLearningApp::GetNoise(float mean, float variance) {
	return 0.0f;
}

void ContactLearningApp::FollowFeelerCamera() {

	float to_translate_x = m_feeler->m_object->GetCOMPosition().x() - m_oldPos.x();
	m_cameraManager->TranslateCamera(RIGHT, to_translate_x);
	m_oldPos = m_feeler->m_object->GetCOMPosition();
}

float ContactLearningApp::SampleSeparation() {
	// TODO
	float separation = m_distribution(m_generator);

	printf("separation = %f\n", separation);

	return separation;
}

void ContactLearningApp::SetSampleParams(float mean_sep, float var) {

	m_distribution = std::normal_distribution<double>(mean_sep, var);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	m_generator = std::default_random_engine(seed);

}

#pragma endregion UTILS

#pragma region DRAWING

void ContactLearningApp::DrawShapeCallback(btScalar *transform, const btCollisionShape *shape, const btVector3 &color) {

	if (shape->getUserPointer() == m_ground)
	{
		// Transform to location
		const btBoxShape *box = static_cast<const btBoxShape*>(shape);
		btVector3 halfSize = box->getHalfExtentsWithMargin();

		glColor3f(color.x(), color.y(), color.z());

		// push the matrix stack
		glPushMatrix();
		glMultMatrixf(transform);

		// BulletOpenGLApplication::DrawShape(transform, shape, color);
		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_ground_texture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);								glVertex3f(-halfSize.x(), -halfSize.y(), -halfSize.z());
		glTexCoord2d(0.0, halfSize.y() );				glVertex3f(-halfSize.x(), halfSize.y(), -halfSize.z());
		glTexCoord2d(halfSize.x(), halfSize.y());	glVertex3f(halfSize.x(), halfSize.y(), -halfSize.z());
		glTexCoord2d(halfSize.x(), 0.0f);				glVertex3f(halfSize.x(), -halfSize.y(), -halfSize.z());
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	}
	else {
		DrawShape(transform, shape, color);
	}

}

void ContactLearningApp::DrawCallback() {

	ContactManager::GetInstance().DrawContactPoints();

	FollowFeelerCamera();

}

void ContactLearningApp::PostTickCallback(btScalar timestep) {

	int numManifolds = m_pWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = m_pWorld->getDispatcher()->getManifoldByIndexInternal(i);

		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		for (int j = 0; j < contactManifold->getNumContacts(); j++)   {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.m_distance1 < 0) {
				// Valid contact point
				//pt.getAppliedImpulse();
				// Go to the position of the contact.
			}
		}
	}
}

void ContactLearningApp::PreTickCallback(btScalar timestep) {

	ContactManager::GetInstance().Update(timestep);
	m_controller->StateLoop();

	ManageBumps();

	// Query to see if moved past the bumps..
	// If moved past bumps, then create more bumps.

}

#pragma endregion DRAWING

void InternalPostTickCallback(btDynamicsWorld *world, btScalar timestep) {
	m_app->PostTickCallback(timestep);
}

void InternalPreTickCallback(btDynamicsWorld *world, btScalar timestep) {
	m_app->PreTickCallback(timestep);
}

static void ContactLearningIdle() {
	m_app->Idle();
}

static void StartButtonPressed() {
	m_app->FeelerStart();
}

static void PauseButtonPressed() {
	m_app->FeelerPause();
}

static void StopButtonPressed() {
	m_app->FeelerStop();
}