#include "RigidBodyGenerator.h"
#include <iostream>
RigidBodyGenerator::RigidBodyGenerator(PxTransform t, PxPhysics* p,int maxNum) : transform(t), maxGenerationNum(maxNum), physics(p)
{
	
}

RigidBodyGenerator::~RigidBodyGenerator()
{
	cout << "Generador de rigid body destruido\n";
}

PxRigidDynamic* RigidBodyGenerator::generateRigidBody()
{
	if (generatedNum >= maxGenerationNum) return nullptr;
	generatedNum++;
	PxRigidDynamic* newSolid;
	newSolid = physics->createRigidDynamic(PxTransform(transform.p.x + (rand()%21 -10), transform.p.y + (rand() % 21 - 10), transform.p.z + (rand() % 21 - 10)));
	newSolid->setLinearVelocity({ 0,50,0 });
	newSolid->setAngularVelocity({ 0,0,0 });
	return newSolid;
}
