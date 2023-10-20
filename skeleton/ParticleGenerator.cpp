#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model, int nParticles, bool oneT) : position(pos), velocity(vel), pModel(model), numParticles(nParticles), oneTime(oneT)
{

}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::setParticle(particleInfo model)
{
}

void ParticleGenerator::setPosition(Vector3 pos)
{
	position = pos;
}

bool ParticleGenerator::shouldDestroy()
{
	return oneTime && alreadyGenerated;
}

