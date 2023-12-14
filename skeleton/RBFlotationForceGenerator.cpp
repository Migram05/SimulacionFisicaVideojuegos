#include "RBFlotationForceGenerator.h"
#include <vector>
#include <iostream>
RBFlotationForceGenerator::RBFlotationForceGenerator(Vector3 o, float d, float g, float dC) : RBForceGenerator(o), density(d), gravity(g), k1(dC)
{

}

void RBFlotationForceGenerator::updateForce(physx::PxRigidDynamic* p)
{
	if (p == nullptr) return;
	addFlotationForce(p);
	addDragForce(p);
}

void RBFlotationForceGenerator::addFlotationForce(physx::PxRigidDynamic* p)
{
	float h = p->getGlobalPose().p.y;
	float h0 = origin.y;
	Vector3 f(0, 0, 0);
	float inmersed = 0;
	physx::PxShape* forma;
	physx::PxShape** formas;
	p->getShapes(&forma, p->getNbShapes());
	float height = forma->getGeometry().box().halfExtents.y;
	if (h - h0 > height * 0.5) {
		inmersed = 0;
	}
	else if (h0 - h > height * 0.5) {
		inmersed = 1;
	}
	else {
		inmersed = (h0 - h) / height + 0.5;
	}
	f.y = density * inmersed * -gravity;
	p->addForce(f);
}

void RBFlotationForceGenerator::addDragForce(physx::PxRigidDynamic* p)
{
	
	Vector3 vel = p->getLinearVelocity();
	float dragCoef = vel.normalize();
	Vector3 dragF;
	dragCoef = k1 * dragCoef;
	dragF = -vel * dragCoef;
	p->addForce(dragF);
}
