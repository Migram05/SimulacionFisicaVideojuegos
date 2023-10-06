#include "ParticleSystem.h"
#include <random>
#include <iostream>

ParticleSystem::ParticleSystem(Vector3 p, Vector3 d) : position(p), direction(d)
{
	pInfo = { p, d, Vector3(0,-9.8,0), 0.98, 10, 20, pT_Spark, Vector4(1,1,0,1), &physx::PxSphereGeometry(1)};
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : particlesList) delete p;
	for (Particle* pt : particlesToDeleteList) delete pt;
}

void ParticleSystem::integrate(double dt)
{
	if (particlesList.size() <= maxNum) {
		pInfo.velocity = direction + Vector3(((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15, ((std::rand() % 201) - 100) / 15);
		particlesList.push_back(new Particle(pInfo));
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
