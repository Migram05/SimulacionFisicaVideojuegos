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
	RenderItem* ground;
	ParticleForceRegistry* registry;
	class GravityGenerator* gGenerator;
	class ParticleDragGenerator* dGenerator;
	class TorbellinoGenerator* tGenerator;
public:
	Scene();
	~Scene();
	void keyPress(unsigned char key);
	void integrate(float dt);
};

