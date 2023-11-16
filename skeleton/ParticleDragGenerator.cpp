#include "ParticleDragGenerator.h"
#include "Particle.h"
#include <iostream>

ParticleDragGenerator::ParticleDragGenerator(Vector3 o, float v1, float v2, float r, bool oneT) : ForceGenerator(o,r, oneT)
{
	k1 = v1; k2 = v2;
}

void ParticleDragGenerator::updateForce(Particle* p, float dt)
{
	if (radius <= 0 || (p->getPos() - origin.p).magnitude() <= radius) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		Vector3 vel = p->getVel();
		float dragCoef = vel.normalize();
		Vector3 dragF;
		dragCoef = k1 * dragCoef + k2 * dragCoef * dragCoef;
		dragF = -vel * dragCoef;
		p->addForce(dragF);
	}
}
