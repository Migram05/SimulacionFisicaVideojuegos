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
	float maxDispersion;
	list<Particle*> pL;
public:
	ParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model,float maxD, int nParticles, bool oneTime);
	~ParticleGenerator();
	void setParticle(particleInfo model);
	void setPosition(Vector3 pos);
	void setMaxDispersion(float newDispersion);
	bool shouldGenerate();
	void setNumparticles(int newNum);
	virtual void generateParticles() = 0;
	list<Particle*> getGeneratedParticles();
};

