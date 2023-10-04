#include "Particle.h"
#include <iostream>

Particle::Particle(particleInfo pI) : lifeTime(pI.lifeTime), dumping(pI.dumping)
{
	pose = physx::PxTransform(pI.origin.x, pI.origin.y, pI.origin.z);
	setParticleValues(pI);
	gravity = pow((velocity.magnitude() / realVelocity.magnitude()), 2) * GRAVITY_VAL;
	mass = pow((realVelocity.magnitude() / velocity.magnitude()), 2) * realMass;
	acceleration = Vector3(0, gravity, 0);
}

Particle::~Particle()
{
	renderItem->release();
}

void Particle::integrate(double dt) //Calculos de velocidad 
{
	timeAlive += dt;
	pose.p += velocity * dt;
	velocity += acceleration * dt;
	velocity *= powf(dumping, dt);
}

bool Particle::checkAlive()
{
	return (timeAlive < lifeTime && pose.p.y > 0); //Comprueba que todavía le queda tiempo de vida y que no ha tocado el suelo
}

void Particle::setParticleValues(const particleInfo i)
{
	//Según el tipo de partícula que queramos se cambian valores de velocidad, color y forma...
	switch (i.type) {
	case pT_Cannon: 
	{
		velocity = i.velocity * 25;
		realVelocity = i.velocity * 250;
		shape = CreateShape(physx::PxSphereGeometry(1));
		renderItem = new RenderItem(shape, &pose, Vector4(1, 0, 0, 1));
		break;
	}
	case pT_Bullet: 
	{
		velocity = i.velocity * 33;
		realVelocity = i.velocity * 330;
		shape = CreateShape(physx::PxSphereGeometry(0.5));
		renderItem = new RenderItem(shape, &pose, Vector4(0, 0, 1, 1));
		break;
	}
	default: break;

	}
}
