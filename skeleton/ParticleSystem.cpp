#include "ParticleSystem.h"
#include <random>
#include <iostream>
#include <string>
#include "ParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"

ParticleSystem::ParticleSystem(Vector3 p, Vector3 d) : position(p), direction(d)
{
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : particlesList) delete p;
	for (Particle* pt : particlesToDeleteList) delete pt;
	for (ParticleGenerator* pG : particleGeneratorList) delete pG;
}

void ParticleSystem::integrate(double dt)
{
	//cout << particlesList.size() << '\n';
	pfr.updateForces();
	list<ParticleGenerator*>::iterator itPG = particleGeneratorList.begin();
	ParticleGenerator* pG;
	while (itPG != particleGeneratorList.end()) { //Se actualizan los generadores
		pG = *itPG;
		if (pG->shouldDestroy()) { //Si el generador solo tiene que generar 1 vez, se destruye 
			particlesGeneratorsToDeleteList.push_back(pG);
			itPG = particleGeneratorList.erase(itPG);
		}
		else {
			particlesList.splice(particlesList.end(), pG->generateParticles()); //Se a�aden las part�culas generadas al generador
			++itPG;
		}
	}
	list<Particle*>::iterator it = particlesList.begin();
	Particle* p;
	while (it != particlesList.end()) { //Se actualizan las part�culas
		p = *it;
		p->integrate(dt);
		if (!p->checkAlive()) { //Se comprueban si se ha acabado su tiempo de vida o han tocado el suelo
			particlesToDeleteList.push_back(p);
			it = particlesList.erase(it);
		}
		else ++it;
	}
	for (Particle* pt : particlesToDeleteList) { //Se borran las part�culas pendientes de destruir
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
