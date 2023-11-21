#include "ParticleForceRegistry.h"
#include "ForceGenerator.h"
#include "Particle.h"
#include <iostream>

ParticleForceRegistry* ParticleForceRegistry::instancePtr = NULL;

ParticleForceRegistry::ParticleForceRegistry()
{
}

void ParticleForceRegistry::addRegistry(ForceGenerator* f, Particle* p)
{
	if (p) {
		forceGeneratorMap[f].insert(p);
		particleMap[p].insert(f);
	}
	else forceGeneratorMap[f];
}

void ParticleForceRegistry::deleteParticleregistry(Particle* p)
{
	if (particleMap.find(p) == particleMap.end()) return;
	auto fSet = particleMap.at(p);
	for (auto fg : fSet) {
		forceGeneratorMap.at(fg).erase(p);
	}
	particleMap.erase(p);
}

void ParticleForceRegistry::deleteForceRegistry(ForceGenerator* f)
{
	if (forceGeneratorMap.find(f) == forceGeneratorMap.end()) return;
	auto pSet = forceGeneratorMap.at(f);
	for (auto p : pSet) {
		particleMap.at(p).erase(f);
	}
	forceGeneratorMap.erase(f);
}

void ParticleForceRegistry::updateForces(float dt)
{
	auto it = forceGeneratorMap.begin();
	while (it != forceGeneratorMap.end()) {
		auto map = *it;
		for (auto p : map.second) {
			map.first->updateForce(p, dt);
		}
		map.first->setEffectDone();
		++it;
	}
}
