#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model) : ParticleGenerator(name, pos, vel, model)
{
}

UniformParticleGenerator::~UniformParticleGenerator()
{
}

list<Particle*> UniformParticleGenerator::generateParticles(int n)
{
	list<Particle*> pL;
	unsigned seed = (rand() % 1001) - 1000;
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<> distribution(1, 10);
	for (int i = 0; i < n; ++i) {
		//pModel.velocity = velocity + Vector3(((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15);
		pModel.origin = position + Vector3(distribution(generator), 0, distribution(generator));
		//pModel.velocity = velocity + Vector3(0, 0, 0);
		pModel.color = { 0,0,1,0 };
		pL.push_back(new Particle(pModel));
	}
	return pL;
}
