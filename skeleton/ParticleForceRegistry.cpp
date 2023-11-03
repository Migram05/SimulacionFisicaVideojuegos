#include "ParticleForceRegistry.h"
#include "ForceGenerator.h"
#include "Particle.h"

ParticleForceRegistry::ParticleForceRegistry()
{
}

void ParticleForceRegistry::addRegistry(ForceGenerator* f, Particle* p)
{
	auto fSetIT = forceGeneratorMap.find(f);
	auto pSet = forceGeneratorMap.at(f);
	pSet.insert(p);
	auto pMapIT = particleMap.find(p);
	auto fSet = particleMap.at(p);
	fSet.insert(f);
}

void ParticleForceRegistry::deleteParticleregistry(Particle* p)
{
	auto fSet = particleMap.at(p);
	for (auto fg : fSet) {
		forceGeneratorMap.at(fg).erase(p);
	}
	particleMap.erase(p);
}

void ParticleForceRegistry::deleteForceRegistry(ForceGenerator* f)
{
	auto pSet = forceGeneratorMap.at(f);
	for (auto p : pSet) {
		deleteParticleregistry(p);
	}
	forceGeneratorMap.erase(f);
}

void ParticleForceRegistry::updateForces()
{
	auto it = forceGeneratorMap.begin();
	while (it != forceGeneratorMap.end()) {
		auto map = *it;
		for (auto p : map.second) {
			map.first->updateForce(p);
		}
		++it;
	}
}
