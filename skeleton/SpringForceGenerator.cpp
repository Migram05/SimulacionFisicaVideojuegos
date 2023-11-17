#include "SpringForceGenerator.h"
#include "Particle.h"

SpringForceGenerator::SpringForceGenerator(Particle* p, float V, float d) : ForceGenerator(p->getPos(), -1, false)
{
	K = V; dR = d;
}

void SpringForceGenerator::updateForce(Particle* p, float dt)
{
	float distancia = (p->getPos() - origin.p).magnitude();
	if (radius <= 0 || distancia <= radius) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		Vector3 springForce(-K * (distancia - dR) * ((p->getPos() - origin.p) / distancia));
		p->addForce(springForce);
	}
}
