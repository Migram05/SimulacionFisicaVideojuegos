#include "ExplosionForceGenerator.h"
#include "Particle.h"
ExplosionForceGenerator::ExplosionForceGenerator(Vector3 o,float V, float r, bool oneT) : ForceGenerator(o,r,oneT)
{
	K = V;
}

void ExplosionForceGenerator::updateForce(Particle* p)
{
	float distancia = (p->getPos() - origin.p).magnitude();
	if (radius <= 0 || distancia <= radius) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		Vector3 explosionForce((K/pow(distancia,2.f)) * Vector3(p->getPos().x - origin.p.x, p->getPos().y - origin.p.y, p->getPos().z - origin.p.z));
		p->addForce(explosionForce);
	}
}
