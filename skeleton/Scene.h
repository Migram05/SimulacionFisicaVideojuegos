#pragma once
#include <vector>
#include <list>
#include "RenderUtils.hpp"

using namespace std;
using namespace physx;

class Scene
{
	class Camera* camera;
	list<class Particle*> particlesList;
	list<class Particle*> particlesToDelete;
public:
	Scene();
	~Scene();
	void keyPress(unsigned char key);
	void integrate(float dt);
};

