#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel)
{
	pose = physx::PxTransform(pos.x, pos.y, pos.z); 
	velocity = vel;
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, Vector4(1,0,0,1));
}

Particle::~Particle()
{
	delete renderItem;
}

void Particle::integrate(double dt)
{
	pose.p = pose.p + velocity * dt;
}
