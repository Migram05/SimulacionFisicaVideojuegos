#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
using namespace physx;
using namespace std;
class RigidBodyGenerator
{
private:
	int maxGenerationNum, generatedNum = 0;
	PxTransform transform;
	PxPhysics* physics;
public:
	RigidBodyGenerator(PxTransform t, PxPhysics* p, int maxNum);
	~RigidBodyGenerator();
	PxRigidDynamic* generateRigidBody();
};

