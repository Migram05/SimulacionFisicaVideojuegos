#pragma once
#include "ForceGenerator.h"
class FlotationForceGenerator : public ForceGenerator
{
protected:
	float density, gravity;
public:
	FlotationForceGenerator(Vector3 o, float d, float g);
	void updateForce(class Particle* p, float dt) override;
};

