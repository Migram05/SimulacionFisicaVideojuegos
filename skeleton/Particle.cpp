#include "Particle.h"
#include <iostream>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 realVel, Vector3 acc, float m, float d = 0.98, float lT = 5) : velocity(vel), acceleration(acc), dumping(d), mass(m), lifeTime(lT)
{
	pose = physx::PxTransform(pos.x, pos.y, pos.z); 
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, Vector4(1,0,0,1));
	gravity = pow((vel.magnitude() / realVel.magnitude()), 2) * GRAVITY_VAL;
}

Particle::~Particle()
{
	delete renderItem;
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
