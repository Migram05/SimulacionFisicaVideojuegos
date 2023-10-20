#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include <string>
#include "Particle.h"
using namespace std;
class ParticleGenerator
{
protected:
	Vector3 position;
	Vector3 velocity;
	particleInfo pModel;
	bool oneTime = false; bool alreadyGenerated = false;
	int numParticles;
public:
	ParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model, int nParticles, bool oneTime);
	~ParticleGenerator();
	void setParticle(particleInfo model);
	void setPosition(Vector3 pos);
	bool shouldDestroy();
	virtual list<Particle*> generateParticles() = 0;
};

