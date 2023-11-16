#include "GravityGenerator.h"
#include "Particle.h"

GravityGenerator::GravityGenerator(Vector3 o, const Vector3& g, float r, bool oneT) : ForceGenerator(o, r, oneT)
{
	gravity = g;
}

void GravityGenerator::updateForce(Particle* p, float dt)
{
	if (radius <= 0 || (p->getPos() - origin.p).magnitude() <= radius) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		p->addForce(gravity * p->getMass());
	}
}
