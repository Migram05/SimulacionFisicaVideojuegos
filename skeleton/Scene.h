#pragma once
#include <vector>
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"

using namespace std;
using namespace physx;

class Scene
{
	class Camera* camera;
	list<class Particle*> particlesList;
	list<class Particle*> particlesToDelete;
	particleInfo spawnParticleInfo;
	RenderItem* ground;
public:
	Scene();
	~Scene();
	void keyPress(unsigned char key);
	void integrate(float dt);
};

