#include "Particle.h"
#include <iostream>

Particle::Particle(particleInfo pI) : lifeTime(pI.lifeTime), dumping(pI.dumping)
{
	pose = physx::PxTransform(pI.origin.x, pI.origin.y, pI.origin.z);
	setParticleValues(pI);
	gravity = pow((velocity.magnitude() / realVelocity.magnitude()), 2) * GRAVITY_VAL;
	mass = pow((realVelocity.magnitude() / velocity.magnitude()), 2) * realMass;
}

Particle::~Particle()
{
	renderItem->release();
}

void Particle::integrate(double dt)
{
	timeAlive += dt;
	pose.p += velocity * dt;
	velocity += acceleration * dt;
	velocity *= powf(dumping, dt);
	velocity.y += gravity * dt;
}

bool Particle::checkAlive()
{
	return timeAlive < lifeTime;
}

void Particle::setParticleValues(const particleInfo i)
{
	switch (i.type) {
	case pT_Bullet: 
	{
		velocity = i.velocity * 25;
		realVelocity = i.velocity * 25;
		acceleration = i.acceleration * 5;
		renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, Vector4(1, 0, 0, 1));
		break;
	}
	default: break;

	}
}
