#pragma once
#include "SpringForceGenerator.h"
class ElasticBandForceGenerator : public SpringForceGenerator
{
public:
	ElasticBandForceGenerator(class Particle* p, float V, float d);
	void updateForce(class Particle* p, float dt) override;
};

