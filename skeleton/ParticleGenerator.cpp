#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model) : position(pos), velocity(vel), pModel(model)
{

}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::setParticle(particleInfo model)
{
}

