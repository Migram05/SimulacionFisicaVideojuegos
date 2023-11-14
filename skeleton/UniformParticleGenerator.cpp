#include "UniformParticleGenerator.h"
#include "ParticleSystem.h"

UniformParticleGenerator::UniformParticleGenerator(ParticleSystem* parentSys, string name, Vector3 pos, float maxDim,Vector3 shape, particleInfo model, int nP, bool oneT) : ParticleGenerator(parentSys,name, pos, model,maxDim, nP, oneT), shape(shape)
{
}

UniformParticleGenerator::~UniformParticleGenerator()
{
}

list<Particle*> UniformParticleGenerator::generateParticles()
{ 
	pL.clear();
	unsigned seed = (rand() % 1001) - 1000;
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<> distribution(0,maxDispersion);
	for (int i = 0; i < numParticles; ++i) {
		pModel.origin = position + Vector3(distribution(generator)*shape.x, distribution(generator) * shape.y, distribution(generator) * shape.z);
		Particle* p = new Particle(pModel, this);
		pL.push_back(p);
	}
	if (oneTime) shouldDestroyItself = true;
	return pL;
}
