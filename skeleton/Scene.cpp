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
Scene::Scene(PxPhysics* gP, PxScene* gS)
{
	//Cámara
	gPhysics = gP; gScene = gS;
	camera = GetCamera();
	camera->setPosition(physx::PxVec3(0, 10, 0));

	//Generadores de fuerza
	registry = ParticleForceRegistry::instance();
	forcesPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	pSystem.push_back(forcesPS);

	gGenerator = new GravityGenerator(Vector3(10, 80, 0), Vector3(0, -98, 0));
	dGenerator = new ParticleDragGenerator(Vector3(10, 100, 0), 1, 0);
	//tGenerator = new TorbellinoGenerator(Vector3(0, 50, 0), 5, 2, 0, 1000);
	//if (tGenerator) forcesPS->addForceGenerator(tGenerator);
	if (dGenerator) forcesPS->addForceGenerator(dGenerator);
	if (gGenerator) forcesPS->addForceGenerator(gGenerator);


	ground = gPhysics->createRigidStatic(PxTransform(0, 0, 0));
	PxShape* forma = CreateShape(PxBoxGeometry(500, 0.1, 500));
	ground->attachShape(*forma);
	gScene->addActor(*ground);
	RenderItem* item = new RenderItem(forma, ground, { 0, 1, 0.25, 1 });

	catapulta1 = gPhysics->createRigidDynamic(PxTransform(GetCamera()->getEye() + PxVec3(1.2, 0, 0.5)));
	PxShape* c1 = CreateShape(PxBoxGeometry(0.1, 0.4, 0.1));
	catapulta1->attachShape(*c1);
	gScene->addActor(*catapulta1);
	RenderItem* r1 = new RenderItem(c1, catapulta1, { 1,1, 1, 1 });
	

	catapulta2 = gPhysics->createRigidDynamic(PxTransform(GetCamera()->getEye() + PxVec3(1.2, 0, -0.5)));
	PxShape* c2 = CreateShape(PxBoxGeometry(0.1, 0.4, 0.1));
	catapulta2->attachShape(*c2);
	gScene->addActor(*catapulta2);
	RenderItem* r2 = new RenderItem(c2, catapulta2, { 1, 1, 1, 1 });

	particleInfo SInfo = { GetCamera()->getEye() + PxVec3(1.2, 0.25, -0.5), Vector3(0,1,0), 0.98, 50, 1000,0, particleType::pT_custom,Vector4(0,0,0,1), CreateShape(physx::PxBoxGeometry(0.05, 0.05, 0.05)), false, 0 };
	spring1 = new Particle(SInfo);

	particleInfo SInfo2 = { GetCamera()->getEye() + PxVec3(1.2, 0.25, 0.5), Vector3(0,1,0), 0.98, 50, 1000,0, particleType::pT_custom,Vector4(0,0,0,1), CreateShape(physx::PxBoxGeometry(0.05, 0.05, 0.05)), false, 0 };
	spring2 = new Particle(SInfo2);

	particleInfo SpringInfo = { GetCamera()->getEye() + PxVec3(1.2, 0, 0), Vector3(0,1,0), 0.98, 50, 1000,0.1, particleType::pT_custom,Vector4(0.5,0,0,1), CreateShape(physx::PxSphereGeometry(0.08)), false, 0 };
	springParticle = new Particle(SpringInfo);
	particlesList.push_back(springParticle);
	sGenerator = new SpringForceGenerator(spring1, 25, 0.01);
	auto sGenerator2 = new SpringForceGenerator(spring2, 25, 0.01);
	if (gGenerator) registry->addRegistry(gGenerator, springParticle);
	if (dGenerator) registry->addRegistry(dGenerator, springParticle);
	registry->addRegistry(sGenerator, springParticle);
	registry->addRegistry(sGenerator2, springParticle);

#ifndef WATER
	//Suelo
	//ground = new RenderItem(CreateShape(physx::PxBoxGeometry(5000, 1, 5000)), Vector4(0, 1, 0.25, 1));
#endif // !WATER
#ifdef WATER
	ground = new RenderItem(CreateShape(physx::PxBoxGeometry(5000, 0.1, 5000)), Vector4(0, 1, 1, 1));
	
#endif // WATER

	/*fireworkPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	pSystem.push_back(fireworkPS);

	forcesPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	pSystem.push_back(forcesPS);

	registry = ParticleForceRegistry::instance();
	gGenerator = new GravityGenerator(Vector3(10, 80, 0), Vector3(0,-98,0));
	dGenerator = new ParticleDragGenerator(Vector3(10, 100, 0), 1, 0);
	tGenerator = new TorbellinoGenerator(Vector3(0, 50, 0),5, 2, 0, 1000);
	
	/*particleInfo newInfo = {Vector3(0,0,0), Vector3(0,1,0), 0.98, 50, 1000,0.5, particleType::pT_custom,Vector4(0.15f,0.15f,0.2f,1), CreateShape(physx::PxSphereGeometry(1)), false, 0};
	auto forcesParticleGenerator = new GaussianParticleGenerator(pSystem[1], "GFuerzas", Vector3(50, 80, 0), newInfo, 5, 1, false);
	forcesPS->addGenerator(forcesParticleGenerator);

	particleInfo newInfo2 = { Vector3(0,0,0), Vector3(0,1,0), 0.98, 50, 1000,0.5, particleType::pT_custom,Vector4(1,1,1,1), CreateShape(physx::PxSphereGeometry(1)), false, 0 };
	auto forcesParticleGenerator2 = new GaussianParticleGenerator(pSystem[1], "GFuerzas2", Vector3(50, 80, 0), newInfo2, 5, 1, false);
	forcesPS->addGenerator(forcesParticleGenerator2);
	
	if (tGenerator) forcesPS->addForceGenerator(tGenerator);
	if (dGenerator) forcesPS->addForceGenerator(dGenerator);
	if (gGenerator) forcesPS->addForceGenerator(gGenerator);
	*/
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

	ground->release();
	catapulta1->release();
	catapulta2->release();
	delete spring1;
	delete spring2;
}

void Scene::keyPress(unsigned char key)
{
	switch(key){
		case 'a': {
			GetCamera()->move(1);
			break;
		}
		case 'd': {
			GetCamera()->move(-1);
			break;
		}
		case ' ':
		{
			
		}
	}
}

void Scene::mousePress(int button, int state)
{
	if (button == 0) {
		attackPressed = !state;
		if (attackPressed) startAttackTime = currentTime;
		else {
			intensidad = PxClamp((currentTime - startAttackTime), 0.4f, maxAttackChargeTime) / maxAttackChargeTime;
			display_text = "";
			shoot(intensidad);
		}
	}
}

void Scene::shoot(float intensity)
{
	//Crea la información para generar un proyectil por defecto
	spawnParticleInfo = { camera->getEye(), camera->getDir(), 0.98, 5, 700,1, particleType::pT_Cannon,Vector4(0,0,1,1), CreateShape(physx::PxSphereGeometry(1)) };
	if (particlesList.size() > maxParticleCount) return;
	spawnParticleInfo.type = pT_Cannon; 
	spawnParticleInfo.velocity = camera->getDir() * intensity;
	spawnParticleInfo.origin = camera->getEye();
	Particle* p = new Particle(spawnParticleInfo);
	particlesList.push_back(p);
	if (gGenerator) registry->addRegistry(gGenerator, p);
	
}

void Scene::integrate(float dt)
{
	currentTime += dt;
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

	if (attackPressed) {
		float v1 = PxClamp((currentTime - startAttackTime), 0.0f, maxAttackChargeTime);
		float i = v1 / maxAttackChargeTime;
		display_text = "";
		for (int c = 0; c < i * 10; ++c) {
			display_text += "/";
		}
		springParticle->setPosition(camera->getEye() + camera->getDir()*1.2 - PxVec3(0, 0.43, 0));
	}
	auto rightVec = camera->getDir().cross({ 0,1,0 });
	catapulta1->setGlobalPose(PxTransform(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + rightVec * 0.7 - PxVec3(0, 0.3, 0)));
	catapulta2->setGlobalPose(PxTransform(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + -rightVec * 0.7 - PxVec3(0, 0.3, 0)));
	spring1->setPosition(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + rightVec * 0.7 - PxVec3(0, 0.1, 0));
	spring2->setPosition(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + -rightVec * 0.7 - PxVec3(0, 0.1, 0));
}

