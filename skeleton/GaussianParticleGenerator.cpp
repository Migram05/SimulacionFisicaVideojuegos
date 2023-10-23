#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model,float maxDim, int nP, bool oneT) : ParticleGenerator(name, pos, vel, model, nP, oneT), maxDimension(maxDim)
{
	
}

GaussianParticleGenerator::~GaussianParticleGenerator()
{
}

void GaussianParticleGenerator::generateParticles()
{
	alreadyGenerated = true;
	unsigned seed = (rand() % 1001) - 1000;
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0,maxDimension);
	for (int i = 0; i < numParticles; ++i) {
		//pModel.velocity = velocity + Vector3(((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15);
		pModel.origin = position + Vector3(distribution(generator), distribution(generator), distribution(generator) );
		pL.push_back(new Particle(pModel));
	}
}
