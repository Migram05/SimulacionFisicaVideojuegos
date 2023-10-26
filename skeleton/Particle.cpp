#include "Particle.h"
#include "ParticleGenerator.h"
#include <iostream>

Particle::Particle(particleInfo pI, ParticleGenerator* pG) : lifeTime(pI.lifeTime), dumping(pI.dumping), origin(pI.origin), maxDistance(pI.maxDistance), acceleration(pI.acceleration), velocity(pI.velocity), generator(pG), generateOnDestroy(pI.destroySpawn), generateNum(pI.destroySpawnNum)
{
	pose = physx::PxTransform(pI.origin.x, pI.origin.y, pI.origin.z);
	setParticleValues(pI);
	gravity = pow((velocity.magnitude() / realVelocity.magnitude()), 2) * GRAVITY_VAL;
	if (gravity == 0) gravity = GRAVITY_VAL;
	mass = pow((realVelocity.magnitude() / velocity.magnitude()), 2) * realMass;
	acceleration = Vector3(acceleration.x, gravity * acceleration.y, acceleration.z);
	pInfoCopy = pI;
}

Particle::~Particle()
{
	//Si una partícula tiene asociado un generador y la propiedad de generar partículas al morir, cuando se destruya, generará partículas
	if (generator && generateOnDestroy) {
		generator->setPosition(pose.p);
		particleInfo copy = pInfoCopy;
		copy.type = pT_custom;
		copy.destroySpawn = false;
		copy.color = { (float)(rand()%100)/100,(float)(rand() % 100) / 100,(float)(rand() % 100) / 100,1};
		for (int i = 0; i < generateNum; i++) {
			copy.lifeTime = rand() % 5 + 1;
			copy.velocity = { (float)(rand() % 16) - 8,(float)(rand() % 5),(float)(rand() % 16) - 8 };
			generator->setNumparticles(1);
			generator->setParticle(copy);
			generator->generateParticles();
		}
	}
	renderItem->release();
}

void Particle::integrate(double dt) //Calculos de velocidad 
{
	timeAlive += dt;
	velocity += acceleration * dt;
	velocity *= powf(dumping, dt);
	pose.p += velocity * dt;
}

bool Particle::checkAlive()
{
	//Comprueba que todavía le queda tiempo de vida y que no ha tocado el suelo y que no está demasiado lejos del origen
	return (timeAlive < lifeTime && pose.p.y > 0 && (pose.p - origin).magnitude() <= maxDistance);
}

void Particle::setParticleValues(const particleInfo i)
{
	//Según el tipo de partícula que queramos se cambian valores de velocidad, color y forma...
	switch (i.type) {
	case pT_Cannon: 
	{
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

Firework::Firework(particleInfo pI, ParticleGenerator* pG, int t) : Particle(pI, pG), type(t)
{
}

Firework::~Firework()
{
	//Al destruirse genera partículas
	if (generator) {
		particleInfo pInfo;
		pInfo.type = pT_custom;
		pInfo.color = { (float)(rand() % 2),(float)(rand() % 2),(float)(rand() % 2),1 };
		pInfo.geometry = CreateShape(physx::PxSphereGeometry(0.15));
		pInfo.destroySpawnNum = 5;
		pInfo.maxDistance = 100;
		pInfo.dumping = 0.1; //Le reducimos el dumping para que caigan más despacio
		if (type < 3) {
			if (type == 2) pInfo.destroySpawn = true; //Si son de tipo 2 lo ajustamos para que las partículas generen más partículas
			generator->setPosition(pose.p);
			for (int i = 0; i < 800; i++) { //Bucle de generación de partículas
				pInfo.lifeTime = rand() % 5 + 1;
				pInfo.velocity = { (float)(rand() % 20) - 10,(float)(rand() % 20),(float)(rand() % 20) - 10 };
				generator->setNumparticles(1);
				generator->setParticle(pInfo);
				generator->generateParticles();
			}
			generator->setMaxDispersion(0.1); 
		}
		else {//Fuego artificial de una cara sonriente 
			pInfo.geometry = CreateShape(physx::PxSphereGeometry(0.5));
			pInfo.lifeTime = 3;
			pInfo.velocity = { 0,0,0 };
			pInfo.dumping = 0.98;
			generator->setParticle(pInfo);
			generator->setNumparticles(1);
			Vector3 eyesOffset(5, 10, 0);
			Vector3 mouth1(5, -7, 0);
			Vector3 mouth2(10, -5, 0);

			Vector3 eye1 = pose.p + eyesOffset;
			Vector3 eye2 (pose.p.x - eyesOffset.x, pose.p.y + eyesOffset.y, pose.p.z);
			Vector3 m1 = pose.p + mouth1;
			Vector3 m2 (pose.p.x - mouth1.x, pose.p.y + mouth1.y, pose.p.z);
			Vector3 m4 = pose.p + mouth2;
			Vector3 m5(pose.p.x - mouth2.x, pose.p.y + mouth2.y, pose.p.z);

			generator->setPosition(eye1);
			generator->generateParticles();
			generator->setPosition(eye2);
			generator->generateParticles();
			generator->setPosition(m1);
			generator->generateParticles();
			generator->setPosition(m2);
			generator->generateParticles();
			generator->setPosition(m4);
			generator->generateParticles();
			generator->setPosition(m5);
			generator->generateParticles();
			generator->setPosition({pose.p.x, pose.p.y + mouth1.y, pose.p.z});
			generator->generateParticles();
		}
	}
}
