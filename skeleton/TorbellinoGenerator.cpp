#include "TorbellinoGenerator.h"
#include "Particle.h"

TorbellinoGenerator::TorbellinoGenerator(Vector3 o,float V, float k1, float k2, float r, bool oneT) : ParticleDragGenerator(o, k1, k2, r, oneT)
{
	K = V;
}

void TorbellinoGenerator::updateForce(Particle* p, float dt)
{
	
	if (radius <= 0 || (p->getPos() - origin.p).magnitude() <= radius) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		Vector3 pPose = p->getPos();
		Vector3 velTorbellino = { -(pPose.z - origin.p.z),50 - (pPose.y - origin.p.y),(pPose.x - origin.p.x) };
		velTorbellino *= K;
		p->addForce(velTorbellino);
	}
	ParticleDragGenerator::updateForce(p, dt); //Se aplica un drag también
}
