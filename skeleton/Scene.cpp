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
			particlesList.push_back(new Particle(camera->getEye(), 25 * camera->getDir(), 25 * camera->getDir(), camera->getDir() * 5, 12, 0.98, 5));
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
}
