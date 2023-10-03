#include "Scene.h"
#include <iostream>

Scene::Scene()
{
	camera = GetCamera();
	spawnParticleInfo = { camera->getEye(), camera->getDir(), camera->getDir(), 0.98, 555, particleType::pT_Cannon };
	ground = new RenderItem(CreateShape(physx::PxBoxGeometry(5000,1,5000)), Vector4(0, 1, 0, 1));
	
}

Scene::~Scene()
{
	for (Particle* p : particlesList) delete p;
	particlesList.clear();
	for (Particle* pt : particlesToDelete) delete pt;
	particlesToDelete.clear();
}

void Scene::keyPress(unsigned char key)
{
	switch(toupper(key)){
		case '1':
		{
			spawnParticleInfo.type = pT_Cannon;
			break;
		}
		case '2':
		{
			spawnParticleInfo.type = pT_Bullet;
			break;
		}
		case ' ':
		{
			spawnParticleInfo.velocity = spawnParticleInfo.acceleration = camera->getDir();
			spawnParticleInfo.origin = camera->getEye();
			particlesList.push_back(new Particle(spawnParticleInfo));
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

