#include "Particle.h"
#include "ParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "ParticleSystem.h"
#include "ParticleForceRegistry.h"
#include <iostream>

Particle::Particle(particleInfo pI, ParticleGenerator* pG) : lifeTime(pI.lifeTime), dumping(pI.dumping), origin(pI.origin), maxDistance(pI.maxDistance), velocity(pI.velocity), generator(pG), generateOnDestroy(pI.destroySpawn), generateNum(pI.destroySpawnNum)
{
	pose = physx::PxTransform(pI.origin.x, pI.origin.y, pI.origin.z);
	setParticleValues(pI);
	
	mass = pow((realVelocity.magnitude() / velocity.magnitude()), 2) * pI.mass;
	invMass = 1 / mass;
	pInfoCopy = pI;
	if (pG != nullptr) pSystem = pG->getSystem();
}

Particle::~Particle()
{
	//Si una partícula tiene asociado un generador y la propiedad de generar partículas al morir, cuando se destruya, generará partículas
	if (pSystem != nullptr && generateOnDestroy) {
		particleInfo copy = pInfoCopy;
		copy.type = pT_custom;
		copy.lifeTime = (rand()%4) + 1;
		copy.destroySpawn = false;
		copy.origin = pose.p;
		generator->setPosition(pose.p);
		for (int i = 0; i < generateNum; i++) {
			copy.lifeTime = rand() % 5 + 1;
			copy.color = { (float)(rand() % 100) / 100,(float)(rand() % 100) / 100,(float)(rand() % 100) / 100,1 };
			copy.velocity = { (float)(rand() % 16) - 8,(float)(rand() % 5),(float)(rand() % 16) - 8 };
			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", pose.p, copy, 0.1, 1, true));
		}
	}
	ParticleForceRegistry::instance()->deleteParticleregistry(this);
	renderItem->release();
}

void Particle::integrate(double dt) //Calculos de velocidad 
{
	if (invMass <= 0.0f) return;
	timeAlive += dt;
	acceleration = totalForce * invMass;
	velocity += acceleration * dt;
	velocity *= powf(dumping, dt);
	pose.p += velocity * dt;

	clearAcumulatedForce();
}

bool Particle::checkAlive()
{
	//Comprueba que todavía le queda tiempo de vida y que no ha tocado el suelo y que no está demasiado lejos del origen
	return (timeAlive < lifeTime && pose.p.y > 0 && (pose.p - origin).magnitude() <= maxDistance);
}

void Particle::addForce(Vector3 f)
{
	totalForce += f;
}

void Particle::clearAcumulatedForce()
{
	totalForce *= 0;
}

void Particle::setParticleValues(const particleInfo i)
{
	//Según el tipo de partícula que queramos se cambian valores de velocidad, color y forma...
	switch (i.type) {
	case pT_Cannon: 
	{
		lifeTime = 5;
		velocity = i.velocity * 200;
		realVelocity = i.velocity * 250;
		shape = CreateShape(physx::PxSphereGeometry(1));
		renderItem = new RenderItem(shape, &pose, Vector4(1, 0, 0, 1));
		break;
	}
	case pT_Bullet: 
	{
		velocity = i.velocity * 290;
		realVelocity = i.velocity * 340;
		shape = CreateShape(physx::PxSphereGeometry(0.5));
		renderItem = new RenderItem(shape, &pose, Vector4(0, 0, 1, 1));
		break;
	}
	case pT_Spark:
	{
		velocity = i.velocity * 10;
		realVelocity = i.velocity * 10;
		shape = CreateShape(physx::PxSphereGeometry(0.1));
		renderItem = new RenderItem(shape, &pose, i.color);
		break;
	}
	case pT_custom:
	{
		velocity = realVelocity = i.velocity;
		shape = i.geometry;
		renderItem = new RenderItem(shape, &pose, i.color);
		break;
	}
	default: break;

	}
}

Firework::Firework(particleInfo pI, ParticleSystem* pS, int t) : Particle(pI), type(t)
{
	pSystem = pS;
}

Firework::~Firework()
{
	//Al destruirse genera partículas
	if (pSystem) {
		particleInfo pInfo;
		pInfo.type = pT_custom;
		pInfo.color = { (float)(rand() % 2),(float)(rand() % 2),(float)(rand() % 2),1 };
		pInfo.geometry = CreateShape(physx::PxSphereGeometry(0.15));
		pInfo.destroySpawnNum = 5;
		pInfo.maxDistance = 1000;
		pInfo.dumping = 0.1; //Le reducimos el dumping para que caigan más despacio
		pInfo.lifeTime = rand() % 5 + 1;
		pInfo.velocity = { 0,0,0 };
		pInfo.origin = pose.p;
		if (type < 3) {
			pInfo.destroySpawn = (type == 2); //Si son de tipo 2 lo ajustamos para que las partículas generen más partículas
			generator = new GaussianParticleGenerator(pSystem, "GenGenerator", pose.p, pInfo, 5,650, type != 1); //Si es de tipo 1 se genera indefinidamente
			pSystem->addGenerator(generator);
		}
		else {//Fuego artificial de una cara sonriente 
			pInfo.geometry = CreateShape(physx::PxSphereGeometry(0.5));
			pInfo.lifeTime = 3;
			pInfo.velocity = { 0,0,0 };
			pInfo.dumping = 0.98;
			Vector3 eyesOffset(5, 10, 0);
			Vector3 mouth1(5, -7, 0);
			Vector3 mouth2(10, -5, 0);

			Vector3 eye1 = pose.p + eyesOffset;
			Vector3 eye2 (pose.p.x - eyesOffset.x, pose.p.y + eyesOffset.y, pose.p.z);
			Vector3 m1 = pose.p + mouth1;
			Vector3 m2 (pose.p.x - mouth1.x, pose.p.y + mouth1.y, pose.p.z);
			Vector3 m4 = pose.p + mouth2;
			Vector3 m5(pose.p.x - mouth2.x, pose.p.y + mouth2.y, pose.p.z);
			Vector3 m6(pose.p.x, pose.p.y + mouth1.y, pose.p.z);

			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", eye1, pInfo, 0.1, 1, true));
			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", eye2, pInfo, 0.1, 1, true));
			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", m1, pInfo, 0.1, 1, true));
			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", m2, pInfo, 0.1, 1, true));
			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", m4, pInfo, 0.1, 1, true));
			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", m5, pInfo, 0.1, 1, true));
			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", m6, pInfo, 0.1, 1, true));
		}
	}
}
