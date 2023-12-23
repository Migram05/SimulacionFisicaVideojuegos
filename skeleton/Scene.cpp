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
	camera->setPosition(physx::PxVec3(0, 1, 0));

	//Generadores de fuerza
	registry = ParticleForceRegistry::instance();
	forcesPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	fireworkPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	pSystem.push_back(forcesPS);
	pSystem.push_back(fireworkPS);

	gGenerator = new GravityGenerator(Vector3(10, 80, 0), Vector3(0, -98, 0));
	dGenerator = new ParticleDragGenerator(Vector3(10, 100, 0), 1, 0);
	//tGenerator = new TorbellinoGenerator(Vector3(0, 50, 0), 5, 2, 0, 1000);
	//if (tGenerator) forcesPS->addForceGenerator(tGenerator);
	
	if (dGenerator) {
		forcesPS->addForceGenerator(dGenerator);
		//fireworkPS->addForceGenerator(dGenerator);
	}
	if (gGenerator) {
		forcesPS->addForceGenerator(gGenerator);
		//fireworkPS->addForceGenerator(gGenerator);
	}


	ground = gPhysics->createRigidStatic(PxTransform(0, 0, 0));
	PxShape* forma = CreateShape(PxBoxGeometry(500, 0.1, 500));
	ground->attachShape(*forma);
	gScene->addActor(*ground);
	ground->setRBType(RBType::RB_Ground);
	RenderItem* item = new RenderItem(forma, ground, { 0, 1, 0.25, 1 });

	//Catapulta
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

	particleInfo SInfo = { GetCamera()->getEye() + PxVec3(1.2, 0.25, -0.5), Vector3(0,1,0), 0.98, -1, 1000,0, particleType::pT_custom,Vector4(0,0,0,1), CreateShape(physx::PxBoxGeometry(0.05, 0.05, 0.05)), false, 0 };
	spring1 = new Particle(SInfo);
	particlesList.push_back(spring1);

	particleInfo SInfo2 = { GetCamera()->getEye() + PxVec3(1.2, 0.25, 0.5), Vector3(0,1,0), 0.98, -1, 1000,0, particleType::pT_custom,Vector4(0,0,0,1), CreateShape(physx::PxBoxGeometry(0.05, 0.05, 0.05)), false, 0 };
	spring2 = new Particle(SInfo2);
	particlesList.push_back(spring2);

	particleInfo SpringInfo = { GetCamera()->getEye() + PxVec3(1.2, 0, 0), Vector3(0,1,0), 0.98, -1, 1000,0.1, particleType::pT_custom,Vector4(0.5,0,0,1), CreateShape(physx::PxSphereGeometry(0.05)), false, 0 };
	springParticle = new Particle(SpringInfo);
	particlesList.push_back(springParticle);
	sGenerator = new SpringForceGenerator(spring1, 25, 0.01);
	auto sGenerator2 = new SpringForceGenerator(spring2, 25, 0.01);
	if (gGenerator) registry->addRegistry(gGenerator, springParticle);
	if (dGenerator) registry->addRegistry(dGenerator, springParticle);
	registry->addRegistry(sGenerator, springParticle);
	registry->addRegistry(sGenerator2, springParticle);

	createLevel(1);

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
	for (PxRigidBody* rb : rigidBodyList) {
		if(rb->isReleasable())  rb->release();
	}
	//ground->release();
	//catapulta1->release();
	//catapulta2->release();
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
		if (attackPressed) {
			startAttackTime = currentTime;
			particleInfo trayectoryPInfo = { springParticle->getPos(), {0,-1,0}, 0.98, 0.5, 1000,0.5, particleType::pT_custom,Vector4(0,0,1,1), CreateShape(physx::PxSphereGeometry(0.03)), false, 0};
			trayectoryGenerator = new GaussianParticleGenerator(forcesPS, "Trayectoria", camera->getEye() + camera->getDir() * 2, trayectoryPInfo, 0.05, 1);
			forcesPS->addGenerator(trayectoryGenerator);
		}
		else {
			trayectoryGenerator->forceDestroy();

			display_text = "";
			shoot(intensidad);
		}
	}
}

void Scene::shoot(float intensity)
{
	if (numDisparos <= 0) {
		return;
	}
	numDisparos--;
	bulletCounter_text = to_string(numDisparos);
	bola = gPhysics->createRigidDynamic(PxTransform(camera->getEye()));
	PxShape* b = CreateShape(PxSphereGeometry(0.2));
	bola->attachShape(*b);
	gScene->addActor(*bola);
	RenderItem* e1 = new RenderItem(b, bola, { 0.5,0.5, 0.5, 1 });
	rigidBodyList.push_back(bola);
	bola->setMass(10);
	bola->addForce(camera->getDir() * 85000 * intensity);
	bola->setRBType(RBType::RB_Projectile);
	bola->canInteractCollisions = true;

	if(numDisparos <= 0) defeatTimer = currentTime;
}

void Scene::levelCompleted()
{
	particleInfo fInfo = { GetCamera()->getEye() + camera->getDir() * 10,Vector3(0,5,0), 0.98, 1, 1000,1, particleType::pT_custom,Vector4(1,0,1,1), CreateShape(physx::PxSphereGeometry(0.05)), true, 50 };
	particlesList.push_back(new Firework(fInfo, fireworkPS));
	particlesList.push_back(new Firework(fInfo, fireworkPS));
	defeatTimer = -1;
	end_text = "VICTORIA";
}

void Scene::createLevel(int lvl)
{
	switch (lvl) {
		case 1: level1(); break;
		default: break;
	}
}

void Scene::level1()
{
	numDisparos = 3;
	bulletCounter_text = to_string(numDisparos);
	PxTransform spawnPos(camera->getEye() + camera->getDir() * 35);
	spawnPos.p.y = 0;

	PxRigidDynamic* pilar1 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x-1,1, spawnPos.p.z-1));
	PxShape* c1 = CreateShape(PxBoxGeometry(0.4, 1, 0.4));
	pilar1->attachShape(*c1);
	gScene->addActor(*pilar1);
	RenderItem* r1 = new RenderItem(c1, pilar1, { 1,1, 1, 1 });
	pilar1->setMass(10);
	rigidBodyList.push_back(pilar1);

	PxRigidDynamic* pilar2 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x - 1, 1, spawnPos.p.z+1));
	PxShape* c2 = CreateShape(PxBoxGeometry(0.4, 1, 0.4));
	pilar2->attachShape(*c2);
	gScene->addActor(*pilar2);
	RenderItem* r2 = new RenderItem(c2, pilar2, { 1,1, 1, 1 });
	pilar2->setMass(10);
	rigidBodyList.push_back(pilar2);

	PxRigidDynamic* pilar3 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x + 1, 1, spawnPos.p.z - 1));
	PxShape* c3 = CreateShape(PxBoxGeometry(0.4, 1, 0.4));
	pilar3->attachShape(*c3);
	gScene->addActor(*pilar3);
	RenderItem* r3 = new RenderItem(c3, pilar3, { 1,1, 1, 1 });
	pilar3->setMass(10);
	rigidBodyList.push_back(pilar3);

	PxRigidDynamic* pilar4 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x + 1, 1, spawnPos.p.z + 1));
	PxShape* c4 = CreateShape(PxBoxGeometry(0.4, 1, 0.4));
	pilar4->attachShape(*c4);
	gScene->addActor(*pilar4);
	RenderItem* r4 = new RenderItem(c4, pilar4, { 1,1, 1, 1 });
	pilar4->setMass(10);
	rigidBodyList.push_back(pilar4);

	PxRigidDynamic* suelo1 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x, 2, spawnPos.p.z));
	PxShape* s = CreateShape(PxBoxGeometry(2, 0.1, 2));
	suelo1->attachShape(*s);
	gScene->addActor(*suelo1);
	RenderItem* s1 = new RenderItem(s, suelo1, { 1,1, 1, 1 });
	suelo1->setMass(5);
	rigidBodyList.push_back(suelo1);

	PxRigidDynamic* enemigo1 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x, 4, spawnPos.p.z));
	PxShape* e = CreateShape(PxBoxGeometry(0.5, 0.5, 0.5));
	enemigo1->attachShape(*e);
	gScene->addActor(*enemigo1);
	RenderItem* e1 = new RenderItem(e, enemigo1, { 1,0, 0, 1 });
	enemigo1->setMass(5);
	enemigo1->setMassSpaceInertiaTensor({ 0,0,0 });
	enemigo1->setRBType(RBType::RB_Enemy);
	rigidBodyList.push_back(enemigo1);
	enemigo1->canInteractCollisions = true;
}

void Scene::integrate(float dt)
{
	//cout << springParticle->getPos().x << ' ' << springParticle->getPos().y << ' ' << springParticle->getPos().z << '\n';
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
		intensidad = PxClamp((currentTime - startAttackTime), 0.4f, maxAttackChargeTime) / maxAttackChargeTime;
		display_text = "";
		for (int c = 0; c < intensidad * 10; ++c) {
			display_text += "/";
		}
		springParticle->setPosition(camera->getEye() + camera->getDir()*1.2 - PxVec3(0, 0.43, 0));
		trayectoryGenerator->setPosition(springParticle->getPos());
		trayectoryGenerator->setParticleVelocity(camera->getDir() * intensidad * 200);
	}
	if (defeatTimer != -1 && currentTime - defeatTimer > 5) end_text = "DERROTA";

	auto rightVec = camera->getDir().cross({ 0,1,0 });
	catapulta1->setGlobalPose(PxTransform(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + rightVec * 0.7 - PxVec3(0, 0.3, 0)));
	catapulta2->setGlobalPose(PxTransform(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + -rightVec * 0.7 - PxVec3(0, 0.3, 0)));
	spring1->setPosition(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + rightVec * 0.7 - PxVec3(0, 0.1, 0));
	spring2->setPosition(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + -rightVec * 0.7 - PxVec3(0, 0.1, 0));
}

void Scene::generateExplosion(Vector3 origin)
{
	for (auto rb : rigidBodyList) {
		if (rb != nullptr) {
			float distancia = (rb->getGlobalPose().p - origin).magnitude();
			if (distancia <= 200) {
				Vector3 posiciones(rb->getGlobalPose().p.x - origin.x, rb->getGlobalPose().p.y - origin.y, rb->getGlobalPose().p.z - origin.z);
				Vector3 explosionForce((50000 / pow(distancia, 2.f)) * posiciones);
				rb->addForce(explosionForce);
			}
		}
	}
}

void Scene::enemyDead()
{
	numEnemies--;
	if (numEnemies <= 0) levelCompleted();
}

