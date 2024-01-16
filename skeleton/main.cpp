#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <iostream>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Scene.h"
#include "RigidBodyGenerator.h"

#include "RBForceGenerator.h"
#include "RBFlotationForceGenerator.h"

std::string display_text = "";
std::string bulletCounter_text = "";
std::string end_text = "";

using namespace physx;
using namespace std;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;
PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;
PxMaterial*				gMaterial	= NULL;
PxPvd*                  gPvd        = NULL;
PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;


Scene* currentScene;



// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);
	
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	currentScene = new Scene(gPhysics, gScene); //Creación de la escena

}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	currentScene->integrate(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
#ifdef PRACTICA5
	if (RBGenerator != nullptr) delete RBGenerator;
#endif // PRACTICA5
#ifndef PRACTICA5
	delete currentScene; //Se borra nuestra escena
#endif
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	currentScene->keyPress(key); //Se llama al método key press de la escena
}

void mousePress(int button, int state) {
	currentScene->mousePress(button, state);
}

void onCollision(physx::PxRigidActor* actor1, physx::PxRigidActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
	if (!actor1 || !actor2) return;
	
	if (currentScene->isBullet(actor1)) { //Cuando una bala colisiona genera una explosión
		if (currentScene->canBulletInteract(actor1)) {
			currentScene->generateExplosion(actor1->getWorldBounds().getCenter());
		}
	}
	else if (currentScene->isEnemy(actor1) && actor2 == currentScene->getGround()) { //Si un enemigo toca el suelo, muere
		if (currentScene->canEnemyInteract(actor1)) {
			currentScene->enemyDead();
		}
	}
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}