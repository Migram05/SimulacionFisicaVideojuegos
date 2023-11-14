#include "Scene.h"
#include <iostream>
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "GravityGenerator.h"
#include "ParticleDragGenerator.h"

Scene::Scene()
{
	camera = GetCamera();
	//Crea la información para generar un proyectil por defecto
	spawnParticleInfo = { camera->getEye(), camera->getDir(), camera->getDir(), 0.98, 5, 700,1, particleType::pT_Cannon,Vector4(1,0,0,1), CreateShape(physx::PxSphereGeometry(1))};
	//Suelo
	ground = new RenderItem(CreateShape(physx::PxBoxGeometry(5000,1,5000)), Vector4(0, 1, 0, 1));
	
	fireworkPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	registry = new ParticleForceRegistry();
	gGenerator = new GravityGenerator();
	dGenerator = new ParticleDragGenerator(0.3, 0);
	pSystem.push_back(fireworkPS);
}

Scene::~Scene()
{
	for (Particle* p : particlesList) {
		delete p;
		registry->deleteParticleregistry(p);
	}
	for (Particle* pt : particlesToDelete) {
		delete pt;
		registry->deleteParticleregistry(pt);
	}
	for (ParticleSystem* pS : pSystem) delete pS;
	registry->deleteForceRegistry(gGenerator);
	registry->deleteForceRegistry(dGenerator);
	delete dGenerator;
	delete gGenerator;
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
			particlesList.push_back(new Firework(spawnParticleInfo, fireworkPS));
			break;
		}
		case '4':
		{
			spawnParticleInfo.type = pT_custom; spawnParticleInfo.color = { 0,0.2,0.5,1 }; spawnParticleInfo.geometry = CreateShape(physx::PxSphereGeometry(1));
			spawnParticleInfo.velocity = { 0,35,0 }; spawnParticleInfo.lifeTime = 3; spawnParticleInfo.acceleration = { 0,1,0 };
			spawnParticleInfo.origin = { 10,40,10 }; spawnParticleInfo.destroySpawnNum = 10;
			particlesList.push_back(new Firework(spawnParticleInfo, fireworkPS, 2));
			break;
		}
		case '5':
		{
			spawnParticleInfo.type = pT_custom; spawnParticleInfo.color = { 1,0.2,0.5,1 }; spawnParticleInfo.geometry = CreateShape(physx::PxSphereGeometry(1));
			spawnParticleInfo.velocity = { 0,35,0 }; spawnParticleInfo.lifeTime = 3; spawnParticleInfo.acceleration = { 0,1,0 };
			spawnParticleInfo.origin = { 10,40,10 };
			particlesList.push_back(new Firework(spawnParticleInfo, fireworkPS, 3));
			break;
		}
		case ' ':
		{
			if (particlesList.size() > maxParticleCount) break;
			//Se actualizan los valores de posición y dirección de la cámara para la creación del proyectil
			//Esto no se utiliza para la práctica de los fuegos artificiales
			spawnParticleInfo.velocity = spawnParticleInfo.acceleration = camera->getDir();
			spawnParticleInfo.origin = camera->getEye();
			spawnParticleInfo.lifeTime = 1;
			spawnParticleInfo.mass = 1;
			Particle* p = new Particle(spawnParticleInfo);
			particlesList.push_back(p);
			registry->addRegistry(gGenerator, p);
			registry->addRegistry(dGenerator, p);
			break;
		}
	}
}

void Scene::integrate(float dt)
{
	list<Particle*>::iterator it = particlesList.begin();
	Particle* p;
	registry->updateForces();
	while (it != particlesList.end()) { //Se actualizan las partículas
		p = *it;
		p->integrate(dt);
		if (!p->checkAlive()) { //Se comprueban si se ha acabado su tiempo de vida o han tocado el suelo
			particlesToDelete.push_back(p);
			it = particlesList.erase(it);
		}
		else ++it;
	}
	for(auto pS : pSystem) pS->integrate(dt);

	for (Particle* pt : particlesToDelete) { //Se borran las partículas pendientes de destruir
		delete pt;
		registry->deleteParticleregistry(pt);
	}
	particlesToDelete.clear();
}

