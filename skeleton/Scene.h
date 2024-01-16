#pragma once
#include <vector>
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleForceRegistry.h"
#include <unordered_map>

//Macro para elegir el nivel
#define CURRENT_LEVEL 1

using namespace std;
using namespace physx;

class Scene
{
	class Camera* camera;
	list<Particle*> particlesList;
	list<Particle*> particlesToDelete;
	int maxParticleCount = 5;
	particleInfo spawnParticleInfo;
	vector< ParticleSystem*> pSystem;
	ParticleSystem* fireworkPS;
	
	ParticleForceRegistry* registry;
	ParticleSystem* forcesPS;
	class GravityGenerator* gGenerator = nullptr;
	class ParticleDragGenerator* dGenerator = nullptr;
	class TorbellinoGenerator* tGenerator = nullptr;
	class SpringForceGenerator* sGenerator = nullptr;
	class FlotationForceGenerator* fGenerator = nullptr;

	class GaussianParticleGenerator* trayectoryGenerator = nullptr;

	PxPhysics* gPhysics;
	PxScene* gScene;

	PxRigidStatic* ground;
	PxRigidDynamic* catapulta1;
	PxRigidDynamic* catapulta2;

	PxRigidDynamic* bola = nullptr;

	Particle* springParticle;
	Particle* spring1;
	Particle* spring2;

	list<PxRigidBody*> rigidBodyList;

	unordered_map<PxActor*, bool> enemies;
	unordered_map<PxActor*, bool> bullets;

	float currentTime = 0;
	float startAttackTime;
	float maxAttackChargeTime = 2;
	float intensidad;
	bool attackPressed = false;
	int numEnemies = 0;
	int numDisparos = 0;
	float defeatTimer = -1;

	void shoot(float intensity);
	void levelCompleted();
	void createLevel(int lvl);
	void level1();
	void level2();
	void generateTrees(int n);
	void windForce(float intensity);
public:
	Scene(PxPhysics* gP, PxScene* gS);
	~Scene();
	void keyPress(unsigned char key);
	void mousePress(int button, int state);
	void integrate(float dt);
	void generateExplosion(Vector3 origin);
	void enemyDead();
	//Métodos usados para la comprobación de colisiones
	PxActor* getGround() { return ground; }
	bool isEnemy(PxActor* e) { return enemies.find(e) != enemies.end(); }
	bool canEnemyInteract(PxActor* e) { bool res = enemies[e]; enemies[e] = false; return res; }
	bool isBullet(PxActor* e) { return bullets.find(e) != bullets.end(); }
	bool canBulletInteract(PxActor* e) { bool res = bullets[e]; bullets[e] = false; return res; }
};

