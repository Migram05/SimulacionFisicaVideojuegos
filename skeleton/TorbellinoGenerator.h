#pragma once
#include "ParticleDragGenerator.h"
class TorbellinoGenerator : public ParticleDragGenerator
{
protected:
	Vector3 origin;
	float K = 1;
public:
	TorbellinoGenerator(Vector3 o, float k1, float k2);
	virtual void updateForce(class Particle* p) override;
};

