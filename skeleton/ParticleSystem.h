#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include "Particle.h"
#include "ParticleForceRegistry.h"


using namespace std;

class ParticleSystem
{
public:
	ParticleSystem(Vector3 p, Vector3 d);
	~ParticleSystem();
	void integrate(double dt);
	void addGenerator(class ParticleGenerator* pG);
private:
	Vector3 position;
	Vector3 direction;
	list<Particle*> particlesList;
	list<Particle*> particlesToDeleteList;
	list<class ParticleGenerator*> particleGeneratorList;
	list<class ParticleGenerator*> particlesGeneratorsToDeleteList;
	list<class ForceGenerator*> forceGeneratorList;
	ParticleForceRegistry pfr;
	int maxNum = 10000;
	particleInfo pInfo;
};

