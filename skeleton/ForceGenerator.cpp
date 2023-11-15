#include "ForceGenerator.h"
#include "ParticleForceRegistry.h"
#include <iostream>

ForceGenerator::ForceGenerator(Vector3 o, float r, bool oT)
{
	origin = physx::PxTransform(o);
	radius = r; oneTimeEffect = oT;
	if (radius > 0) {
		physx::PxReal rR = radius;
		std::cout << rR << '\n';
		auto shape = CreateShape(physx::PxSphereGeometry(rR));
		renderItem = new RenderItem(shape, &origin, Vector4(1, 0, 0, 0.2));
	}
}

ForceGenerator::~ForceGenerator()
{
	ParticleForceRegistry::instance()->deleteForceRegistry(this);
}
