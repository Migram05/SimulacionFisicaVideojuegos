#pragma once
#include "RBForceGenerator.h"
class RBFlotationForceGenerator : public RBForceGenerator
{
protected:
	float density, gravity, k1;
public:
	RBFlotationForceGenerator(Vector3 o, float d, float g, float dC);
	~RBFlotationForceGenerator();
	void updateForce(physx::PxRigidDynamic* p) override;
private:
	void addFlotationForce(physx::PxRigidDynamic* p);
	void addDragForce(physx::PxRigidDynamic* p);
};

