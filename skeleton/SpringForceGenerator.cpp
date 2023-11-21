#include "SpringForceGenerator.h"
#include "Particle.h"

SpringForceGenerator::SpringForceGenerator(Particle* p, float V, float d) : ForceGenerator(p->getPos(), -1, false)
{
	K = V; dR = d; otherParticle = p;
}

void SpringForceGenerator::updateForce(Particle* p, float dt)
{
	if (fabs(p->getInvMass()) < 1e-10) return;
	Vector3 relativePosVector =  (otherParticle->getPos() - p->getPos());
	float length = relativePosVector.normalize();
	float delta_x = length - dR;
	Vector3 springForce(relativePosVector * delta_x * K);
	p->addForce(springForce);
}
