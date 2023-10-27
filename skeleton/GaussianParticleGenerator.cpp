#include "GaussianParticleGenerator.h"
#include "ParticleSystem.h"

GaussianParticleGenerator::GaussianParticleGenerator(ParticleSystem* parentSys, string name, Vector3 pos, particleInfo model,float maxDim, int nP, bool oneT) : ParticleGenerator(parentSys,name, pos, model,maxDim, nP, oneT)
{
	
}

GaussianParticleGenerator::~GaussianParticleGenerator()
{
}

list<Particle*> GaussianParticleGenerator::generateParticles()
{
	pL.clear();
	unsigned seed = (rand() % 1001) - 1000;
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0,maxDispersion);
	for (int i = 0; i < numParticles; ++i) {
		pModel.origin = position + Vector3(distribution(generator), distribution(generator), distribution(generator) );
		pL.push_back(new Particle(pModel, this));
	}
	if (oneTime) shouldDestroyItself = true;
	return pL;
}
