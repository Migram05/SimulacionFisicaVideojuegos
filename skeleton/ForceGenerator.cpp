#include "ForceGenerator.h"
#include "ParticleForceRegistry.h"
#include <iostream>

ForceGenerator::ForceGenerator(Vector3 o, float r, bool oT)
{
	origin = physx::PxTransform(o);
	radius = r; oneTimeEffect = oT;
	if (radius > 0) { //Si el radio es negativo o nulo, el efecto del generador es global
		physx::PxReal rR = radius;
		//std::cout << rR << '\n';
		forceSphere = physx::PxSphereGeometry(rR);
		forceShape = CreateShape(forceSphere);
		renderItem = new RenderItem(forceShape, &origin, Vector4(1, 0, 0, 0.2));
	}
}

ForceGenerator::~ForceGenerator()
{
	ParticleForceRegistry::instance()->deleteForceRegistry(this);
}
