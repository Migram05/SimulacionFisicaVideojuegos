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
#include "UniformTreeGenerator.h"

Scene::Scene(PxPhysics* gP, PxScene* gS)
{
	//Cámara
	gPhysics = gP; gScene = gS;
	camera = GetCamera();
	camera->setPosition(physx::PxVec3(0, 1, 0));

	//Generadores de fuerza para las partículas
	registry = ParticleForceRegistry::instance();
	forcesPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	fireworkPS = new ParticleSystem(Vector3(0, 40, 0), Vector3(0, 1, 0));
	pSystem.push_back(forcesPS);
	pSystem.push_back(fireworkPS);

	gGenerator = new GravityGenerator(Vector3(10, 80, 0), Vector3(0, -98, 0));
	dGenerator = new ParticleDragGenerator(Vector3(10, 100, 0), 1, 0);
	
	if (dGenerator) {
		forcesPS->addForceGenerator(dGenerator);
	}
	if (gGenerator) {
		forcesPS->addForceGenerator(gGenerator);
	}

	//Suelo del nivel
	ground = gPhysics->createRigidStatic(PxTransform(0, 0, 0));
	PxShape* forma = CreateShape(PxBoxGeometry(500, 0.1, 500));
	ground->attachShape(*forma);
	gScene->addActor(*ground);
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

	//Generación del nivel
	createLevel(CURRENT_LEVEL);
}	


Scene::~Scene() //Borrado de todas las entidades de la escena
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
	bullets.clear();
	enemies.clear();
}

void Scene::keyPress(unsigned char key)//Input de teclado, ver código de la cámara para más información
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
	}
}

void Scene::mousePress(int button, int state) //Input del ratón
{
	if (button == 0) {
		attackPressed = !state;
		if (attackPressed) { //Si se pulsa el clic, empieza un contador que calcula la intensidad, y se crea un generador de partículas para la trayectoria
			startAttackTime = currentTime;
			particleInfo trayectoryPInfo = { springParticle->getPos(), {0,-1,0}, 0.98, 0.5, 1000,0.5, particleType::pT_custom,Vector4(0,0,1,1), CreateShape(physx::PxSphereGeometry(0.03)), false, 0};
			trayectoryGenerator = new GaussianParticleGenerator(forcesPS, "Trayectoria", camera->getEye() + camera->getDir() * 2, trayectoryPInfo, 0.05, 1);
			forcesPS->addGenerator(trayectoryGenerator);
		}
		else { //Si se suelta, se destruye el generador, y se dispara
			trayectoryGenerator->forceDestroy();
			display_text = "";
			shoot(intensidad);
		}
	}
}

void Scene::shoot(float intensity) //Método que construye la bola
{
	if (numDisparos <= 0) { //Hay un máximo de disparos por nivel
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
	bullets[bola] = true;
	if(numDisparos <= 0) defeatTimer = currentTime; //En caso de que no queden más bolas, empieza el temporizador de derrota
}

void Scene::levelCompleted() //En caso de ganar se crean 2 fuegos artificiales
{
	particleInfo fInfo = { GetCamera()->getEye() + camera->getDir() * 10,Vector3(0,5,0), 0.98, 1, 1000,1, particleType::pT_custom,Vector4(1,0,1,1), CreateShape(physx::PxSphereGeometry(0.05)), true, 50 };
	particlesList.push_back(new Firework(fInfo, fireworkPS));
	particlesList.push_back(new Firework(fInfo, fireworkPS));
	defeatTimer = -1;
	end_text = "VICTORIA";
}

void Scene::createLevel(int lvl) //Creador de los niveles
{
	switch (lvl) {
	case 1: level1(); generateTrees(100); break;
	case 2: level2(); generateTrees(70); break;
		default: break;
	}
	numEnemies = enemies.size();
}

void Scene::level1() //Generador del nivel 1
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
	enemies[enemigo1] = true;
	rigidBodyList.push_back(enemigo1);
}

void Scene::level2() //Generador del nivel 2
{
	numDisparos = 4;
	bulletCounter_text = to_string(numDisparos);
	PxTransform spawnPos(camera->getEye() + camera->getDir() * 35);
	spawnPos.p.y = 0;

	PxRigidDynamic* pilar1 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x - 2, 2, spawnPos.p.z));
	PxShape* c1 = CreateShape(PxBoxGeometry(0.4, 1.5, 0.4));
	pilar1->attachShape(*c1);
	gScene->addActor(*pilar1);
	RenderItem* r1 = new RenderItem(c1, pilar1, { 1,1, 1, 1 });
	pilar1->setMass(30);
	rigidBodyList.push_back(pilar1);


	PxRigidDynamic* suelo1 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x-2, 4.5, spawnPos.p.z));
	PxShape* s = CreateShape(PxBoxGeometry(2, 0.1, 2));
	suelo1->attachShape(*s);
	gScene->addActor(*suelo1);
	RenderItem* s1 = new RenderItem(s, suelo1, { 1,1, 1, 1 });
	suelo1->setMass(15);
	rigidBodyList.push_back(suelo1);

	PxRigidDynamic* enemigo1 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x-2, 5.5, spawnPos.p.z));
	PxShape* e = CreateShape(PxBoxGeometry(0.5, 0.5, 0.5));
	enemigo1->attachShape(*e);
	gScene->addActor(*enemigo1);
	RenderItem* e1 = new RenderItem(e, enemigo1, { 1,0, 0, 1 });
	enemigo1->setMass(5);
	enemigo1->setMassSpaceInertiaTensor({ 0,0,0 });
	enemies[enemigo1] = true;
	rigidBodyList.push_back(enemigo1);

	PxRigidDynamic* pilar2 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x -3, 3.5, spawnPos.p.z + 10));
	PxShape* c2 = CreateShape(PxBoxGeometry(0.4, 2.5, 0.4));
	pilar2->attachShape(*c2);
	gScene->addActor(*pilar2);
	RenderItem* r2 = new RenderItem(c2, pilar2, { 1,1, 1, 1 });
	pilar2->setMass(30);
	rigidBodyList.push_back(pilar2);


	PxRigidDynamic* suelo2 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x - 3, 5.5, spawnPos.p.z + 10));
	PxShape* s2 = CreateShape(PxBoxGeometry(2, 0.1, 2));
	suelo2->attachShape(*s2);
	gScene->addActor(*suelo2);
	RenderItem* s3 = new RenderItem(s2, suelo2, { 1,1, 1, 1 });
	suelo2->setMass(15);
	rigidBodyList.push_back(suelo2);

	PxRigidDynamic* enemigo2 = gPhysics->createRigidDynamic(PxTransform(spawnPos.p.x - 3, 6.5, spawnPos.p.z + 10));
	PxShape* e2 = CreateShape(PxBoxGeometry(0.5, 0.5, 0.5));
	enemigo2->attachShape(*e2);
	gScene->addActor(*enemigo2);
	RenderItem* e3 = new RenderItem(e2, enemigo2, { 1,0, 0, 1 });
	enemigo2->setMass(5);
	enemigo2->setMassSpaceInertiaTensor({ 0,0,0 });
	enemies[enemigo2] = true;
	rigidBodyList.push_back(enemigo2);
}

void Scene::generateTrees(int num) //Generador de áboles (estético)
{
	// Usa un generador uniforme para colocar los árboles en el mapa
	ParticleSystem* treePS = new ParticleSystem(Vector3(0, 0, 0), Vector3(200, 3, 200));
	pSystem.push_back(treePS);
	particleInfo treeModel = { Vector3(0,1,0), Vector3(0,0,0), 0.98, -1, 1000,0, particleType::pT_custom,Vector4(0,1,0,1), CreateShape(physx::PxSphereGeometry(2)), false, 0};
	UniformTreeGenerator* treeGenerator = new UniformTreeGenerator(treePS, "GeneradorArboles", Vector3(-100, 2,-100), 300, 0, treeModel, num);
	treePS->addGenerator(treeGenerator);

	particleInfo sunModel = { Vector3(150,150,100), Vector3(0,0,0), 0.98, -1, 1000,0, particleType::pT_custom,Vector4(1,1,0,1), CreateShape(physx::PxSphereGeometry(20)), false, 0 };
	Particle* sun = new Particle(sunModel);
	particlesList.push_back(sun);
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
		intensidad = PxClamp((currentTime - startAttackTime), 0.4f, maxAttackChargeTime) / maxAttackChargeTime;
		display_text = "";
		for (int c = 0; c < intensidad * 10; ++c) {
			display_text += "/"; //Se muestra en la pantalla la intensidad de lanzamiento
		}
		springParticle->setPosition(camera->getEye() + camera->getDir()*1.2 - PxVec3(0, 0.43, 0)); //Se estira el muelle de la catapulta
		trayectoryGenerator->setPosition(springParticle->getPos()); //Se ajusta la posición del generador de trayectoria para que esté en frente del jugador
		trayectoryGenerator->setParticleVelocity(camera->getDir() * intensidad * 200);
	}
	if (defeatTimer != -1 && currentTime - defeatTimer > 5) end_text = "DERROTA"; //Si después de usar la última bala, pasan 5 segundos, pierdes

	//Se ajustan los objetos de la catapulta para que se muevan con el jugador
	auto rightVec = camera->getDir().cross({ 0,1,0 });
	catapulta1->setGlobalPose(PxTransform(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + rightVec * 0.7 - PxVec3(0, 0.3, 0)));
	catapulta2->setGlobalPose(PxTransform(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + -rightVec * 0.7 - PxVec3(0, 0.3, 0)));
	spring1->setPosition(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + rightVec * 0.7 - PxVec3(0, 0.1, 0));
	spring2->setPosition(GetCamera()->getEye() + GetCamera()->getDir() * 1.5 + -rightVec * 0.7 - PxVec3(0, 0.1, 0));

	if (CURRENT_LEVEL == 2) { //En el nivel 2 se aplica una fuerza de viento a las balas
		windForce(-((rand() % 20)+40));
	}
}

void Scene::generateExplosion(Vector3 origin) //Generador de explosiones para RB
{
	for (auto rb : rigidBodyList) {
		if (rb != nullptr) {
			float distancia = (rb->getGlobalPose().p - origin).magnitude(); 
			if (distancia <= 200) { //Comprueba que la distancia no es superior al alcance
				Vector3 posiciones(rb->getGlobalPose().p.x - origin.x, rb->getGlobalPose().p.y - origin.y, rb->getGlobalPose().p.z - origin.z);
				Vector3 explosionForce((40000 / pow(distancia, 2.f)) * posiciones);
				rb->addForce(explosionForce);
			}
		}
	}
}

void Scene::windForce(float intensity) //Generador de fuerza del viento para RB
{
	for (auto rb : rigidBodyList) {
		if ((bullets.find(rb))!= bullets.end()) { //Se aplica solo a las bolas de cañon (comprobación constante)
			Vector3 windForce(0, 0, intensity);
			rb->addForce(windForce);
		}
	}
}


void Scene::enemyDead() //Comprabdor de que quedan enemigos vivos
{
	numEnemies--;
	if (numEnemies <= 0) levelCompleted();
}

