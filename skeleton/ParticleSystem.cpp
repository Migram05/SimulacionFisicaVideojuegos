#include "ParticleSystem.h"
#include <random>
#include <iostream>
#include <string>
#include "ParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"

ParticleSystem::ParticleSystem(Vector3 p, Vector3 d) : position(p), direction(d)
{
	pInfo = { p, d, Vector3(0,1,0), 0.98, 10, 100, pT_Spark, Vector4(1,1,0,1), &physx::PxSphereGeometry(1)};
	particleGeneratorList.push_back(new UniformParticleGenerator("G1",position, direction, pInfo));
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : particlesList) delete p;
	for (Particle* pt : particlesToDeleteList) delete pt;
	for (ParticleGenerator* pG : particleGeneratorList) delete pG;
}

void ParticleSystem::integrate(double dt)
{
	for (auto pG : particleGeneratorList) {
		if (particlesList.size() <= maxNum) particlesList.merge(pG->generateParticles(1));
	}
	list<Particle*>::iterator it = particlesList.begin();
	Particle* p;
	while (it != particlesList.end()) { //Se actualizan las partículas
		p = *it;
		p->integrate(dt);
		if (!p->checkAlive()) { //Se comprueban si se ha acabado su tiempo de vida o han tocado el suelo
			particlesToDeleteList.push_back(p);
			it = particlesList.erase(it);
		}
		else ++it;
	}
	for (Particle* pt : particlesToDeleteList) { //Se borran las partículas pendientes de destruir
		delete pt;
	}
	particlesToDeleteList.clear();
}
