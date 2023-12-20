#pragma once
#include <vector>
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleForceRegistry.h"

using namespace std;
using namespace physx;

class Scene
{
	class Camera* camera;
	list<Particle*> particlesList;
	list<Particle*> particlesToDelete;
	int maxParticleCount = 5;
	particleInfo spawnParticleInfo;
	vector< ParticleSystem*> pSystem;
	ParticleSystem* fireworkPS;
	
	ParticleForceRegistry* registry;
	ParticleSystem* forcesPS;
	class GravityGenerator* gGenerator = nullptr;
	class ParticleDragGenerator* dGenerator = nullptr;
	class TorbellinoGenerator* tGenerator = nullptr;
	class SpringForceGenerator* sGenerator = nullptr;
	class FlotationForceGenerator* fGenerator = nullptr;

	PxPhysics* gPhysics;
	PxScene* gScene;

	PxRigidStatic* ground;
	PxRigidDynamic* catapulta1;
	PxRigidDynamic* catapulta2;

	Particle* springParticle;
	Particle* spring1;
	Particle* spring2;

	float currentTime = 0;
	float startAttackTime;
	float maxAttackChargeTime = 2;
	float intensidad;
	bool attackPressed = false;

	void shoot(float intensity);
public:
	Scene(PxPhysics* gP, PxScene* gS);
	~Scene();
	void keyPress(unsigned char key);
	void mousePress(int button, int state);
	void integrate(float dt);
};

