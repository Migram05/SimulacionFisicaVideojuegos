#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model, int nP, bool oneT) : ParticleGenerator(name, pos, vel, model, nP, oneT)
{
	
}

GaussianParticleGenerator::~GaussianParticleGenerator()
{
}

list<Particle*> GaussianParticleGenerator::generateParticles()
{
	alreadyGenerated = true;
	list<Particle*> pL;
	unsigned seed = (rand() % 1001) - 1000;
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0.0,5);
	for (int i = 0; i < numParticles; ++i) {
		//pModel.velocity = velocity + Vector3(((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15);
		pModel.origin = position + Vector3(distribution(generator), distribution(generator), distribution(generator) );
		pModel.color = { 0,0,1,0 };
		pL.push_back(new Particle(pModel));
	}
	return pL;
}
