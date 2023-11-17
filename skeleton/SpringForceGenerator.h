#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator
{
protected:
	float K, dR;
public:
	SpringForceGenerator(class Particle* p, float V, float d);
	void updateForce(class Particle* p, float dt) override;
};

