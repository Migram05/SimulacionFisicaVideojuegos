#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(string name, Vector3 pos, Vector3 vel, float maxDim,Vector3 shape, particleInfo model, int nP, bool oneT) : ParticleGenerator(name, pos, vel, model, nP, oneT), maxDimension(maxDim), shape(shape)
{
}

UniformParticleGenerator::~UniformParticleGenerator()
{
}

void UniformParticleGenerator::generateParticles()
{ 
	alreadyGenerated = true;
	unsigned seed = (rand() % 1001) - 1000;
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<> distribution(0,maxDimension);
	for (int i = 0; i < numParticles; ++i) {
		pModel.origin = position + Vector3(distribution(generator)*shape.x, distribution(generator) * shape.y, distribution(generator) * shape.z);
		pL.push_back(new Particle(pModel, this));
	}
}
