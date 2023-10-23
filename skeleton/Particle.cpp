#include "Particle.h"
#include "ParticleGenerator.h"
#include <iostream>

Particle::Particle(particleInfo pI, ParticleGenerator* pG) : lifeTime(pI.lifeTime), dumping(pI.dumping), origin(pI.origin), maxDistance(pI.maxDistance), acceleration(pI.acceleration), velocity(pI.velocity), generator(pG)
{
	pose = physx::PxTransform(pI.origin.x, pI.origin.y, pI.origin.z);
	setParticleValues(pI);
	gravity = pow((velocity.magnitude() / realVelocity.magnitude()), 2) * GRAVITY_VAL;
	if (gravity == 0) gravity = GRAVITY_VAL;
	mass = pow((realVelocity.magnitude() / velocity.magnitude()), 2) * realMass;
	acceleration = Vector3(acceleration.x, gravity * acceleration.y, acceleration.z);
}

Particle::~Particle()
{
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

Firework::Firework(particleInfo pI, ParticleGenerator* pG) : Particle(pI, pG)
{
}

Firework::~Firework()
{
	if (generator) {
		generator->setPosition(pose.p);
		particleInfo pInfo; 
		pInfo.type = pT_custom;
		pInfo.color = { (float)(rand() % 2),(float)(rand() % 2),(float)(rand() % 2),1 };
		pInfo.geometry = CreateShape(physx::PxSphereGeometry(0.5));
		pInfo.lifeTime = 3;
		pInfo.origin = pose.p;
		pInfo.acceleration = { 0,1,0 };
		pInfo.dumping = 0.98;
		pInfo.maxDistance = 100;
		for (int i = 0; i < 50; i++) {
			pInfo.velocity = { (float)(rand()%15) -15,(float)(rand() % 20),(float)(rand() % 15) - 15 };
			generator->setNumparticles(1);
			generator->setParticle(pInfo);
			generator->generateParticles();
		}
		
	}
}
