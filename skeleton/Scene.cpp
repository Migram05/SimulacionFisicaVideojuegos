#include "Scene.h"
#include <iostream>
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"

Scene::Scene()
{
	camera = GetCamera();
	//Crea la informaci�n para generar un proyectil por defecto
	spawnParticleInfo = { camera->getEye(), camera->getDir(), camera->getDir(), 0.98, 5, 700, particleType::pT_Cannon,Vector4(1,0,0,1), CreateShape(physx::PxSphereGeometry(1))};
	//Suelo
	ground = new RenderItem(CreateShape(physx::PxBoxGeometry(5000,1,5000)), Vector4(0, 1, 0, 1));
	
	fireworkPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	pSystem.push_back(fireworkPS);
}

Scene::~Scene()
{
	for (Particle* p : particlesList) delete p;
	for (Particle* pt : particlesToDelete) delete pt;
	for (ParticleSystem* pS : pSystem) delete pS;
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
		case '3':
		{
			spawnParticleInfo.type = pT_custom; spawnParticleInfo.color = { 0,1,1,1 }; spawnParticleInfo.geometry = CreateShape(physx::PxSphereGeometry(1));
			spawnParticleInfo.velocity = { 0,35,0 }; spawnParticleInfo.lifeTime = 3; spawnParticleInfo.acceleration = { 0,1,0 };
			spawnParticleInfo.origin = { 10,40,10 };
			ParticleGenerator* pG = new GaussianParticleGenerator("G1", { 0,30,0 }, { 0,1,0 }, spawnParticleInfo,0.1, 0, true);
			fireworkPS->addGenerator(pG);
			particlesList.push_back(new Firework(spawnParticleInfo, pG));
			break;
		}
		case ' ':
		{
			if (particlesList.size() > maxParticleCount) break;
			//Se actualizan los valores de posici�n y direcci�n de la c�mara para la creaci�n del proyectil
			spawnParticleInfo.velocity = spawnParticleInfo.acceleration = camera->getDir();
			spawnParticleInfo.origin = camera->getEye();
			spawnParticleInfo.lifeTime = 1;
			particlesList.push_back(new Particle(spawnParticleInfo));
			break;
		}
	}
}

void Scene::integrate(float dt)
{
	list<Particle*>::iterator it = particlesList.begin();
	Particle* p;
	while (it != particlesList.end()) { //Se actualizan las part�culas
		p = *it;
		p->integrate(dt);
		if (!p->checkAlive()) { //Se comprueban si se ha acabado su tiempo de vida o han tocado el suelo
			particlesToDelete.push_back(p);
			it = particlesList.erase(it);
		}
		else ++it;
	}
	for(auto pS : pSystem) pS->integrate(dt);

	for (Particle* pt : particlesToDelete) { //Se borran las part�culas pendientes de destruir
		delete pt;
	}
	particlesToDelete.clear();
}

