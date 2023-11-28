#include "Scene.h"
#include <iostream>
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "GravityGenerator.h"
#include "ParticleDragGenerator.h"
#include "TorbellinoGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "ElasticBandForceGenerator.h"
#include "FlotationForceGenerator.h"
//#define MSTATIC
//#define MMOVIL
//#define SLINKY
//#define WATER
Scene::Scene()
{
	camera = GetCamera();
	
	//Crea la información para generar un proyectil por defecto
	spawnParticleInfo = { camera->getEye(), camera->getDir(), 0.98, 5, 700,1, particleType::pT_Cannon,Vector4(1,0,0,1), CreateShape(physx::PxSphereGeometry(1))};
#ifndef WATER
	//Suelo
	ground = new RenderItem(CreateShape(physx::PxBoxGeometry(5000, 1, 5000)), Vector4(0, 1, 0, 1));
#endif // !WATER
#ifdef WATER
	ground = new RenderItem(CreateShape(physx::PxBoxGeometry(5000, 0.1, 5000)), Vector4(0, 1, 1, 1));
	camera->setPosition(physx::PxVec3(camera->getEye().x, 10, camera->getEye().z));
#endif // WATER

	fireworkPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	pSystem.push_back(fireworkPS);

	forcesPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	pSystem.push_back(forcesPS);

	registry = ParticleForceRegistry::instance();
	gGenerator = new GravityGenerator(Vector3(10, 80, 0), Vector3(0,-98,0));
	dGenerator = new ParticleDragGenerator(Vector3(10, 100, 0), 1, 0);
	tGenerator = new TorbellinoGenerator(Vector3(0, 50, 0),5, 2, 0, 1000);
	
	particleInfo newInfo = {Vector3(0,0,0), Vector3(0,1,0), 0.98, 50, 1000,0.5, particleType::pT_custom,Vector4(0.15f,0.15f,0.2f,1), CreateShape(physx::PxSphereGeometry(1)), false, 0};
	auto forcesParticleGenerator = new GaussianParticleGenerator(pSystem[1], "GFuerzas", Vector3(50, 80, 0), newInfo, 5, 1, false);
	forcesPS->addGenerator(forcesParticleGenerator);

	particleInfo newInfo2 = { Vector3(0,0,0), Vector3(0,1,0), 0.98, 50, 1000,0.5, particleType::pT_custom,Vector4(1,1,1,1), CreateShape(physx::PxSphereGeometry(1)), false, 0 };
	auto forcesParticleGenerator2 = new GaussianParticleGenerator(pSystem[1], "GFuerzas2", Vector3(50, 80, 0), newInfo2, 5, 1, false);
	forcesPS->addGenerator(forcesParticleGenerator2);
	
	if (tGenerator) forcesPS->addForceGenerator(tGenerator);
	if (dGenerator) forcesPS->addForceGenerator(dGenerator);
	if (gGenerator) forcesPS->addForceGenerator(gGenerator);
#ifdef WATER
	if (gGenerator) {
		fGenerator = new FlotationForceGenerator(Vector3(0, 0, 0), 1000, gGenerator->getGravity().y);
		forcesPS->addForceGenerator(fGenerator);
	}
#endif // WATER


#ifdef MSTATIC
	particleInfo SSInfo2 = { camera->getEye() + (camera->getDir() * 15), Vector3(0,1,0), 0.98, 50, 1000,0, particleType::pT_custom,Vector4(0,1,0,1), CreateShape(physx::PxBoxGeometry(1, 1, 1)), false, 0};
	Particle* p = new Particle(SSInfo2);

	particleInfo SpringInfo = { camera->getEye() + (camera->getDir() * 10), Vector3(0,1,0), 0.98, 50, 1000,1, particleType::pT_custom,Vector4(1,1,0,1), CreateShape(physx::PxSphereGeometry(1)), false, 0 };
	Particle* p2 = new Particle(SpringInfo);
	particlesList.push_back(p2);
	sGenerator = new SpringForceGenerator(p, 8, 0.5);
	if (gGenerator) registry->addRegistry(gGenerator, p2);
	if (dGenerator) registry->addRegistry(dGenerator,p2);
	registry->addRegistry(sGenerator, p2);
#endif
#ifdef MMOVIL
	particleInfo SSInfo2 = { camera->getEye() + (camera->getDir() * 15), Vector3(0,1,0), 0.98, 50, 1000,1, particleType::pT_custom,Vector4(0,1,0,1), CreateShape(physx::PxSphereGeometry(1)), false, 0 };
	Particle* p = new Particle(SSInfo2);
	particlesList.push_back(p);
	particleInfo SpringInfo = { camera->getEye() + (camera->getDir() * 10), Vector3(0,1,0), 0.98, 50, 1000,1, particleType::pT_custom,Vector4(1,1,0,1), CreateShape(physx::PxSphereGeometry(1)), false, 0 };
	Particle* p2 = new Particle(SpringInfo);
	particlesList.push_back(p2);
	if (dGenerator) {
		registry->addRegistry(dGenerator, p2);
		registry->addRegistry(dGenerator, p);
	}

	/*sGenerator = new SpringForceGenerator(p, 8, 0.5);
	SpringForceGenerator * sGenerator2 = new SpringForceGenerator(p2, 8, 0.5);
	registry->addRegistry(sGenerator, p2);
	registry->addRegistry(sGenerator2, p);*/
	ElasticBandForceGenerator* eBGenerator = new ElasticBandForceGenerator(p, 8, 10);
	ElasticBandForceGenerator* eBGenerator2 = new ElasticBandForceGenerator(p2, 8, 10);
	registry->addRegistry(eBGenerator, p2);
	registry->addRegistry(eBGenerator2, p);
#endif
#ifdef SLINKY
	Vector3 spawnPos = camera->getEye() + (camera->getDir() * 15) +Vector3(0,15,0);
	float separation = 15;
	particleInfo slinkyPInfo1 = { spawnPos, Vector3(0,1,0), 0.98, 50, 1000,0.2,particleType::pT_custom,Vector4(1,0,0,1), CreateShape(physx::PxSphereGeometry(1)), false, 0 };
	Particle* p = new Particle(slinkyPInfo1);
	particlesList.push_back(p);
	particleInfo slinkyPInfo2 = { spawnPos - Vector3(0, -separation, 0), Vector3(0,1,0), 0.98, 50, 1000,0.2, particleType::pT_custom,Vector4(1,1,0,1), CreateShape(physx::PxSphereGeometry(1)), false, 0};
	Particle* p2 = new Particle(slinkyPInfo2);
	particlesList.push_back(p2);
	particleInfo slinkyPInfo3 = { spawnPos - Vector3(0, -2*separation, 0), Vector3(0,1,0), 0.98, 50, 1000,0.2, particleType::pT_custom,Vector4(0,1,0,1), CreateShape(physx::PxSphereGeometry(1)), false, 0 };
	Particle* p3 = new Particle(slinkyPInfo3);
	particlesList.push_back(p3);
	particleInfo slinkyPInfo4 = { spawnPos - Vector3(0, -3 * separation, 0), Vector3(0,1,0), 0.98, 50, 1000,0.2, particleType::pT_custom,Vector4(0,1,1,1), CreateShape(physx::PxSphereGeometry(1)), false, 0 };
	Particle* p4 = new Particle(slinkyPInfo4);
	particlesList.push_back(p4);
	if (dGenerator) {
		registry->addRegistry(dGenerator, p2);
		registry->addRegistry(dGenerator, p);
		registry->addRegistry(dGenerator, p3);
		registry->addRegistry(dGenerator, p4);
	}
	if (gGenerator) {
		registry->addRegistry(gGenerator, p2);
		registry->addRegistry(gGenerator, p);
		registry->addRegistry(gGenerator, p3);
		registry->addRegistry(gGenerator, p4);
	}
	SpringForceGenerator* sGenerator1 = new SpringForceGenerator(p, 8, 0.5);
	SpringForceGenerator* sGenerator2 = new SpringForceGenerator(p2, 8, 0.5);
	SpringForceGenerator* sGenerator3 = new SpringForceGenerator(p3, 8, 0.5);
	SpringForceGenerator* sGenerator4 = new SpringForceGenerator(p4, 8, 0.5);
	registry->addRegistry(sGenerator1, p2);
	registry->addRegistry(sGenerator2, p);
	registry->addRegistry(sGenerator2, p3);
	registry->addRegistry(sGenerator3, p2);
	registry->addRegistry(sGenerator3, p4);
	registry->addRegistry(sGenerator4, p3);
	
#endif
}	


Scene::~Scene()
{
	for (Particle* p : particlesList) {
		delete p;
	}
	for (Particle* pt : particlesToDelete) {
		delete pt;
	}
	for (ParticleSystem* pS : pSystem) delete pS;
}

void Scene::keyPress(unsigned char key)
{
	switch(key){
		case '1':
		{
			auto springExplosion = new ExplosionForceGenerator(camera->getEye() + (camera->getDir() * 15), 100000, 10, false);
			forcesPS->addForceGenerator(springExplosion);
			for (auto p : particlesList) {
				registry->addRegistry(springExplosion, p);
			}
			break;
		}
		case '2':
		{
			if (particlesList.size() > maxParticleCount ||!gGenerator) break;
			particleInfo waterCube = { camera->getEye() + (camera->getDir() * 15) + Vector3(0,20,0), Vector3(0,1,0), 0.5, 50, 100,999, particleType::pT_custom,Vector4(1,0,0,1), CreateShape(physx::PxBoxGeometry(1,1,1)), false, 0};
			Particle* p = new Particle(waterCube);
			particlesList.push_back(p);
			if (gGenerator) {
				registry->addRegistry(gGenerator, p);
				registry->addRegistry(fGenerator, p);
			}
			if (dGenerator) {
				registry->addRegistry(dGenerator, p);
			}
			break;
		}
		case '+' :{
			sGenerator->setK(sGenerator->getK() + 0.5);
			break;
		}
		case '-': {
			sGenerator->setK(sGenerator->getK() - 0.5);
			if (sGenerator->getK() < 0) sGenerator->setK(0);
			break;
		}
		case '3':
		{
			/*spawnParticleInfo.type = pT_custom; spawnParticleInfo.color = { 0,1,1,1 }; spawnParticleInfo.geometry = CreateShape(physx::PxSphereGeometry(1));
			spawnParticleInfo.velocity = { 0,35,0 }; spawnParticleInfo.lifeTime = 3;
			spawnParticleInfo.origin = { 10,40,10 };
			particlesList.push_back(new Firework(spawnParticleInfo, fireworkPS));*/
			break;
		}
		case '4':
		{
			/*spawnParticleInfo.type = pT_custom; spawnParticleInfo.color = { 0,0.2,0.5,1 }; spawnParticleInfo.geometry = CreateShape(physx::PxSphereGeometry(1));
			spawnParticleInfo.velocity = { 0,35,0 }; spawnParticleInfo.lifeTime = 3;
			spawnParticleInfo.origin = { 10,40,10 }; spawnParticleInfo.destroySpawnNum = 10;
			particlesList.push_back(new Firework(spawnParticleInfo, fireworkPS, 2));*/
			break;
		}
		case '5':
		{
			/*spawnParticleInfo.type = pT_custom; spawnParticleInfo.color = { 1,0.2,0.5,1 }; spawnParticleInfo.geometry = CreateShape(physx::PxSphereGeometry(1));
			spawnParticleInfo.velocity = { 0,35,0 }; spawnParticleInfo.lifeTime = 3;
			spawnParticleInfo.origin = { 10,40,10 };
			particlesList.push_back(new Firework(spawnParticleInfo, fireworkPS, 3));*/
			break;
		}
		case '6':
		{
			auto explosionTest = new ExplosionForceGenerator(Vector3(0, 80, 50), 1000000, 50, false);
			forcesPS->addForceGenerator(explosionTest);
			break;
		}
		case ' ':
		{
			if (particlesList.size() > maxParticleCount) break;
			//Se actualizan los valores de posición y dirección de la cámara para la creación del proyectil
			//Esto no se utiliza para la práctica de los fuegos artificiales
			spawnParticleInfo.type = pT_Cannon; spawnParticleInfo.color = { 1,0,0,1 };
			spawnParticleInfo.velocity = camera->getDir();
			spawnParticleInfo.origin = camera->getEye();
			spawnParticleInfo.lifeTime = 1;
			spawnParticleInfo.mass = 10;
			Particle* p = new Particle(spawnParticleInfo);
			particlesList.push_back(p);
			if(gGenerator) registry->addRegistry(gGenerator, p);
			if(dGenerator) registry->addRegistry(dGenerator, p);
			if(tGenerator) registry->addRegistry(tGenerator, p);
			break;
		}
	}
}

void Scene::integrate(float dt)
{
	list<Particle*>::iterator it = particlesList.begin();
	Particle* p;
	registry->updateForces(dt);
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
	}
	particlesToDelete.clear();
}

