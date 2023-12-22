#include "ParticleGenerator.h"
#include "ParticleSystem.h"

ParticleGenerator::ParticleGenerator(ParticleSystem* parentSys, string name, Vector3 pos, particleInfo model,float maxD, int nParticles, bool oneT, float lT) : position(pos), pModel(model), numParticles(nParticles), oneTime(oneT), maxDispersion(maxD), parentSystem(parentSys), lifeTime(lT)
{
	shouldDestroyItself = false;
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::setParticle(particleInfo model)
{
	pModel = model;
}

void ParticleGenerator::setPosition(Vector3 pos)
{
	position = pos;
}

void ParticleGenerator::setMaxDispersion(float newDispersion)
{
	maxDispersion = newDispersion;
}


bool ParticleGenerator::shouldDestroy()
{
	return shouldDestroyItself;
}

void ParticleGenerator::setNumparticles(int newNum)
{
	numParticles = newNum;
}

ParticleSystem* ParticleGenerator::getSystem()
{
	return parentSystem;
}

list<Particle*> ParticleGenerator::getGeneratedParticles()
{
	auto copy = pL;
	pL.clear();
	if (oneTime) shouldDestroyItself = true;
	return copy;
}

