#include "Scene.h"
#include "Particle.h"
#include <iostream>

Scene::Scene()
{
	camera = GetCamera();
}

Scene::~Scene()
{
	for (Particle* p : particlesList) delete p;
}

void Scene::keyPress(unsigned char key)
{
	switch(toupper(key)){
		case ' ':
		{
			particleInfo pI = { camera->getEye(), camera->getDir(), camera->getDir(), 0.98, 5, particleType::pT_Bullet };
			particlesList.push_back(new Particle(pI));
			break;
		}
	}
}

void Scene::integrate(float dt)
{
	list<Particle*>::iterator it = particlesList.begin();
	Particle* p;
	while (it != particlesList.end()) {
		p = *it;
		p->integrate(dt);
		if (!p->checkAlive()) {
			particlesToDelete.push_back(p);
			it = particlesList.erase(it);
		}
		else ++it;
	}
	for (Particle* pt : particlesToDelete) {
		delete pt;
	}
	particlesToDelete.clear();
}

