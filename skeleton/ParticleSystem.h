#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include "Particle.h"

using namespace std;

class ParticleSystem
{
public:
	ParticleSystem(Vector3 p, Vector3 d);
	~ParticleSystem();
	void integrate(double dt);
private:
	Vector3 position;
	Vector3 direction;
	list<Particle*> particlesList;
	list<Particle*> particlesToDeleteList;
	int maxNum = 300;
	particleInfo pInfo;
};

