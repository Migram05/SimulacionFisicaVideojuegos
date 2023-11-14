#include "TorbellinoGenerator.h"
#include "Particle.h"

TorbellinoGenerator::TorbellinoGenerator(Vector3 o, float k1, float k2) : ParticleDragGenerator(k1, k2)
{
	origin = o;
}

void TorbellinoGenerator::updateForce(Particle* p)
{
	Vector3 pPose = p->getPos();
	Vector3 velTorbellino = {-(pPose.z-origin.z),50-(pPose.y-origin.y),(pPose.x-origin.x)};
	velTorbellino *= K;
	p->addForce(velTorbellino);
}
