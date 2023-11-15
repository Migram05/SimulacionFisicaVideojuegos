#include "ForceGenerator.h"
#include "ParticleForceRegistry.h"

ForceGenerator::~ForceGenerator()
{
	ParticleForceRegistry::instance()->deleteForceRegistry(this);
}
