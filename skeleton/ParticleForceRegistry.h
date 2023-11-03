#pragma once
#include <unordered_set>
#include <unordered_map>
using namespace std;
class ParticleForceRegistry
{
private:
	unordered_map<class ForceGenerator*, unordered_set<class Particle*>> forceGeneratorMap;
	unordered_map<class Particle*, unordered_set<class ForceGenerator*>> particleMap;
public:
	ParticleForceRegistry();
	void addRegistry(class ForceGenerator* f, class Particle* p);
	void deleteParticleregistry(class Particle* p);
	void deleteForceRegistry(class ForceGenerator* f);
	void updateForces();
};

