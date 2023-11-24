#include "ElasticBandForceGenerator.h"
#include "Particle.h"
ElasticBandForceGenerator::ElasticBandForceGenerator(Particle* p, float V, float d) : SpringForceGenerator(p,V,d)
{

}
void ElasticBandForceGenerator::updateForce(Particle* p, float dt)
{
	if (fabs(p->getInvMass()) < 1e-10) return;
	Vector3 relativePosVector = (otherParticle->getPos() - p->getPos());
	float length = relativePosVector.normalize();
	if (length <= dR) return;
	float delta_x = length - dR;
	Vector3 springForce(relativePosVector * delta_x * K);
	p->addForce(springForce);
}
