#include "FlotationForceGenerator.h"
#include "Particle.h"
#include <iostream>
FlotationForceGenerator::FlotationForceGenerator(Vector3 o, float d, float g) : ForceGenerator(o), density(d), gravity(g)
{

}

void FlotationForceGenerator::updateForce(Particle* p, float dt)
{
	float h = p->getPos().y;
	float h0 = origin.p.y;
	Vector3 f(0, 0, 0);
	float inmersed = 0;
	float height = p->getShape()->getGeometry().box().halfExtents.y;
	if (h - h0 > height * 0.5) {
		inmersed = 0;
	}
	else if (h0 - h > height * 0.5) {
		inmersed = 1;
	}
	else {
		inmersed = (h0 - h) / height + 0.5;
	}
	f.y = density * abs(h0-h)* inmersed * -gravity;
	p->addForce(f);
}
