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
	/*if (pSystem != nullptr && generateOnDestroy) {
		particleInfo copy = pInfoCopy;
		copy.type = pT_custom;
		copy.lifeTime = (rand()%4) + 1;
		copy.destroySpawn = false;
		copy.destroySpawnNum = 5;
		copy.origin = pose.p;
		generator->setPosition(pose.p);
		for (int i = 0; i < generateNum; i++) {
			copy.lifeTime = rand() % 5 + 1;
			copy.color = { (float)(rand() % 100) / 100,(float)(rand() % 100) / 100,(float)(rand() % 100) / 100,1 };
			copy.velocity = { (float)(rand() % 16) - 8,(float)(rand() % 5),(float)(rand() % 16) - 8 };
			pSystem->addGenerator(new GaussianParticleGenerator(pSystem, "GenGenerator", pose.p, copy, 0.1, 1, true));
		}
	}*/
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
	if (lifeTime == -1) return true; //Las partículas con tiempo de vida -1 son persistentes
	return (timeAlive < lifeTime && pose.p.y > -1 && (pose.p - origin).magnitude() <= maxDistance);
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
		renderItem = new RenderItem(shape, &pose, Vector4(0.5, 0.5, 0.5, 1));
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

Firework::Firework(particleInfo pI, ParticleSystem* pS) : Particle(pI)
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
		pInfo.geometry = CreateShape(physx::PxSphereGeometry(0.05));
		pInfo.destroySpawnNum = 2;
		pInfo.maxDistance = 1000;
		pInfo.dumping = (20+(float)(rand()%30))/100; //Le reducimos el dumping para que caigan más despacio
		pInfo.lifeTime = rand() % 5 + 1;
		pInfo.velocity = { 0,-1,0 };
		pInfo.origin = pose.p;
		pInfo.destroySpawn = false;

		generator = new GaussianParticleGenerator(pSystem, "GenGenerator", pose.p, pInfo, 1.5, 10, false, 5);
		pSystem->addGenerator(generator);
		
	}
}
