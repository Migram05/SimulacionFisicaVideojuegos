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
public:
	ParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model);
	~ParticleGenerator();
	void setParticle(particleInfo model);
	virtual list<Particle*> generateParticles(int n) = 0;
};

