#include "ParticleSystem.h"
#include <random>
#include <iostream>
#include <string>
#include "ParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"

ParticleSystem::ParticleSystem(Vector3 p, Vector3 d) : position(p), direction(d)
{
	pInfo = { p, d, Vector3(0,1,0), 0.98, 10, 100, pT_Spark, Vector4(0,0,1,1), &physx::PxSphereGeometry(1)};
	particleGeneratorList.push_back(new GaussianParticleGenerator("G1", position, direction, pInfo, 100, true));
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : particlesList) delete p;
	for (Particle* pt : particlesToDeleteList) delete pt;
	for (ParticleGenerator* pG : particleGeneratorList) delete pG;
}

void ParticleSystem::integrate(double dt)
{
	list<ParticleGenerator*>::iterator itPG = particleGeneratorList.begin();
	ParticleGenerator* pG;
	while (itPG != particleGeneratorList.end()) { //Se actualizan los generadores
		pG = *itPG;
		particlesList.splice(particlesList.end(), pG->generateParticles()); //Se añaden las partículas generadas al generador
		if (pG->shouldDestroy()) { //Los generadores que solo tienen que crear 1 vez se destruyen
			particlesGeneratorsToDeleteList.push_back(pG);
			itPG = particleGeneratorList.erase(itPG);
		}
		else ++itPG;
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
	for (ParticleGenerator* pG : particlesGeneratorsToDeleteList) { //Se borran los generadores pendientes de destruir
		delete pG;
	}
	particlesGeneratorsToDeleteList.clear();
}

void ParticleSystem::addGenerator(ParticleGenerator* pG)
{
	particleGeneratorList.push_back(pG);
}
