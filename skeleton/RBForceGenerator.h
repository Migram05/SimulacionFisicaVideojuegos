#pragma once
#include "core.hpp"
using namespace std;
class RBForceGenerator
{
protected:
	Vector3 origin;
public:
	RBForceGenerator(Vector3 o);
	virtual void updateForce(physx::PxRigidDynamic* p) = 0;
};

