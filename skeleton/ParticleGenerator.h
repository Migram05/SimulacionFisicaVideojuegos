#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include <string>
#include "Particle.h"
using namespace std;
class ParticleGenerator
{
protected:
	Vector3 position;
	particleInfo pModel;
	bool oneTime = false, shouldDestroyItself = false;
	int numParticles;
	float maxDispersion;
	list<Particle*> pL;
	class ParticleSystem* parentSystem;
	float lifeTime, spawnTime = -1;
public:
	ParticleGenerator(class ParticleSystem* parentSystem,string name, Vector3 pos, particleInfo model,float maxD, int nParticles, bool oneTime, float lifeTime = -1);
	~ParticleGenerator();
	void setParticle(particleInfo model);
	void setPosition(Vector3 pos);
	void setMaxDispersion(float newDispersion);
	void setParticleVelocity(Vector3 newVel) { pModel.velocity = newVel; }
	bool shouldDestroy();
	void setNumparticles(int newNum);
	inline void setSpawnTime(float sT) { spawnTime = sT; }
	inline float getTimeSpawn() { return spawnTime; }
	inline float getLifeTime() { return lifeTime; }
	inline void forceDestroy() { shouldDestroyItself = true; }
	class ParticleSystem* getSystem();
	virtual list<Particle*> generateParticles() = 0;
	list<Particle*> getGeneratedParticles();
};

