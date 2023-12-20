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

//#define PRACTICA5

std::string display_text = "";


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
vector<PxRigidDynamic*> rbVector;

RigidBodyGenerator* RBGenerator;

Scene* currentScene;

RBFlotationForceGenerator* RBWaterForceGenerator;
bool waterSimulationActive = false;

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
#ifndef PRACTICA5
	currentScene = new Scene(gPhysics, gScene); //Creación de la escena
#endif // !PRACTICA5
#ifdef PRACTICA5
	//ajustamos la cámara
	Camera* camera = GetCamera();
	camera->setPosition({ -20, 15,0 });
	//Creación del suelo
	PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform(0, 0, 0));
	PxShape* forma = CreateShape(PxBoxGeometry(500, 0.1, 500));
	suelo->attachShape(*forma);
	gScene->addActor(*suelo);
	RenderItem* item = new RenderItem(forma, suelo, { 1,1,1,1 });
	RBGenerator = new RigidBodyGenerator({ 0,10,0 },gPhysics, 1000);
	
	RBWaterForceGenerator = new RBFlotationForceGenerator({ 0,20,0 }, 50, sceneDesc.gravity.y, 7);
#endif // PRACTICA5
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
#ifndef PRACTICA5
	currentScene->integrate(t);
#endif // !PRACTICA5
#ifdef PRACTICA5
	if (RBGenerator != nullptr) {
		PxRigidDynamic* RB = RBGenerator->generateRigidBody();
		rbVector.push_back(RB);
		if (RB == nullptr) {
			delete RBGenerator;
			RBGenerator = nullptr;
		}
		else {
			PxShape* ShapeAD;
			int r = rand() % 2;
			ShapeAD = CreateShape(PxBoxGeometry(1, 1, 1));
			RB->attachShape(*ShapeAD);
			PxReal density = rand() % 10; density++;
			PxRigidBodyExt::updateMassAndInertia(*RB, density);
			gScene->addActor(*RB);
			RenderItem* dynamicSolid;
			dynamicSolid = new RenderItem(ShapeAD, RB, { (float)(10-density)/10,(float)(10 - density) / 10,(float)(10 - density) / 10,1});
		}
	}
	if (waterSimulationActive) {
		for (auto rb : rbVector) {
			RBWaterForceGenerator->updateForce(rb);
		}
	}
#endif // PRACTICA5

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


void generateExplosion() {
	PxTransform origin = { GetCamera()->getEye().x,GetCamera()->getEye().y,GetCamera()->getEye().z };
	for (auto rb : rbVector) {
		if (rb != nullptr) {
			float distancia = (rb->getGlobalPose().p - origin.p).magnitude();
			if (distancia <= 200) {
				Vector3 posiciones(rb->getGlobalPose().p.x - origin.p.x, rb->getGlobalPose().p.y - origin.p.y, rb->getGlobalPose().p.z - origin.p.z);
				Vector3 explosionForce((1000000 / pow(distancia, 2.f)) * posiciones);
				rb->addForce(explosionForce);
			}
		}
	}
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


void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
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