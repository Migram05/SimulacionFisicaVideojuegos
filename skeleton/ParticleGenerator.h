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
	particleInfo pModel;
	bool oneTime = false, shouldDestroyItself = false;
	int numParticles;
	float maxDispersion;
	list<Particle*> pL;
	class ParticleSystem* parentSystem;
public:
	ParticleGenerator(class ParticleSystem* parentSystem,string name, Vector3 pos, particleInfo model,float maxD, int nParticles, bool oneTime);
	~ParticleGenerator();
	void setParticle(particleInfo model);
	void setPosition(Vector3 pos);
	void setMaxDispersion(float newDispersion);
	bool shouldDestroy();
	void setNumparticles(int newNum);
	class ParticleSystem* getSystem();
	virtual list<Particle*> generateParticles() = 0;
	list<Particle*> getGeneratedParticles();
};

