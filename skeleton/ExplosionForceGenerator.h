#pragma once
#include "ForceGenerator.h"
class ExplosionForceGenerator : public ForceGenerator
{
protected:
	float K;
public:
	ExplosionForceGenerator(Vector3 o,float V, float r = -1, bool oneT = true);
	virtual void updateForce(class Particle* p) override;
};

