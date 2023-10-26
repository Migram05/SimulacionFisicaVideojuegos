#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model,float maxD, int nParticles, bool oneT) : position(pos), velocity(vel), pModel(model), numParticles(nParticles), oneTime(oneT), maxDispersion(maxD)
{

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

bool ParticleGenerator::shouldGenerate()
{
	return (!oneTime && alreadyGenerated);
}

void ParticleGenerator::setNumparticles(int newNum)
{
	numParticles = newNum;
}

list<Particle*> ParticleGenerator::getGeneratedParticles()
{
	auto copy = pL;
	pL.clear();
	return copy;
}

