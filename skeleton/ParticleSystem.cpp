#include "ParticleSystem.h"
#include <random>
#include <iostream>
#include <string>
#include "ParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "ForceGenerator.h"
#include "ParticleForceRegistry.h"

ParticleSystem::ParticleSystem(Vector3 p, Vector3 d) : position(p), direction(d)
{
	registry = ParticleForceRegistry::instance();
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : particlesList) {
		delete p;
	}
	for (Particle* pt : particlesToDeleteList) {
		delete pt;
	}
	for (ParticleGenerator* pG : particleGeneratorList) delete pG;
	for (auto fG : forceGeneratorList) {
		delete fG;
	}
}

void ParticleSystem::integrate(double dt)
{
	list<ParticleGenerator*>::iterator itPG = particleGeneratorList.begin();
	ParticleGenerator* pG;
	while (itPG != particleGeneratorList.end()) { //Se actualizan los generadores
		pG = *itPG;
		if (pG->shouldDestroy()) { //Si el generador solo tiene que generar 1 vez, se destruye 
			particlesGeneratorsToDeleteList.push_back(pG);
			itPG = particleGeneratorList.erase(itPG);
		}
		else {
			auto listP = pG->generateParticles();
			particlesList.splice(particlesList.end(), listP); //Se añaden las partículas generadas al generador
			++itPG;
		}
	}
	list<Particle*>::iterator it = particlesList.begin();
	Particle* p;
	while (it != particlesList.end()) { //Se actualizan las partículas
		p = *it;
		p->integrate(dt);
		for(auto fG : forceGeneratorList) registry->addRegistry(fG, p);
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

	auto fGIT = forceGeneratorList.begin();
	while (fGIT != forceGeneratorList.end()) {
		auto fG = *fGIT;
		if (fG->shouldDestroy()) { //Si el generador de fuerza se tiene que destruir, se elimina de la lista de generadores de fuerza
			ParticleForceRegistry::instance()->deleteForceRegistry(fG);
			delete fG;
			fGIT = forceGeneratorList.erase(fGIT);
		}
		else fGIT++;
	}
}

void ParticleSystem::addGenerator(ParticleGenerator* pG)
{
	particleGeneratorList.push_back(pG);
}

void ParticleSystem::addForceGenerator(ForceGenerator* fG)
{
	forceGeneratorList.push_back(fG);
	ParticleForceRegistry::instance()->addRegistry(fG);
}
