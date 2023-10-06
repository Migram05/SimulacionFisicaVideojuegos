#include "Scene.h"
#include <iostream>

Scene::Scene()
{
	camera = GetCamera();
	//Crea la información para generar un proyectil por defecto
	spawnParticleInfo = { camera->getEye(), camera->getDir(), camera->getDir(), 0.98, 5, 700, particleType::pT_Cannon,Vector4(1,0,0,1), &physx::PxSphereGeometry(1)};
	//Suelo
	ground = new RenderItem(CreateShape(physx::PxBoxGeometry(5000,1,5000)), Vector4(0, 1, 0, 1));
	
	pSystem = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
}

Scene::~Scene()
{
	for (Particle* p : particlesList) delete p;
	for (Particle* pt : particlesToDelete) delete pt;
	delete pSystem;
}

void Scene::keyPress(unsigned char key)
{
	switch(toupper(key)){
		case '1':
		{
			spawnParticleInfo.type = pT_Cannon; spawnParticleInfo.color = { 1,0,0,1 };
			break;
		}
		case '2':
		{
			spawnParticleInfo.type = pT_Bullet; spawnParticleInfo.color = { 0,0,1,1 };
			break;
		}
		case ' ':
		{
			if (particlesList.size() > maxParticleCount) break;
			//Se actualizan los valores de posición y dirección de la cámara para la creación del proyectil
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
	while (it != particlesList.end()) { //Se actualizan las partículas
		p = *it;
		p->integrate(dt);
		if (!p->checkAlive()) { //Se comprueban si se ha acabado su tiempo de vida o han tocado el suelo
			particlesToDelete.push_back(p);
			it = particlesList.erase(it);
		}
		else ++it;
	}
	pSystem->integrate(dt);
	for (Particle* pt : particlesToDelete) { //Se borran las partículas pendientes de destruir
		delete pt;
	}
	particlesToDelete.clear();
}

