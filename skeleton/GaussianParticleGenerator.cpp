#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model) : ParticleGenerator(name, pos, vel, model)
{
	
}

GaussianParticleGenerator::~GaussianParticleGenerator()
{
}

list<Particle*> GaussianParticleGenerator::generateParticles(int n)
{
	list<Particle*> pL;
	unsigned seed = (rand() % 1001) - 1000;
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0.0, 0.5);
	for (int i = 0; i < n; ++i) {
		//pModel.velocity = velocity + Vector3(((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15);
		pModel.velocity = velocity + Vector3(distribution(generator)*(std::rand() % 4 - 2), 1, distribution(generator) * (std::rand() % 4 - 2));
		pModel.color = { 0,0,1,0 };
		pL.push_back(new Particle(pModel));
	}
	return pL;
}
