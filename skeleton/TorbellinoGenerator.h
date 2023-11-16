#pragma once
#include "ParticleDragGenerator.h"
class TorbellinoGenerator : public ParticleDragGenerator
{
protected:
	float K = 1;
public:
	TorbellinoGenerator(Vector3 o, float V, float k1, float k2, float r = -1, bool oneT = false);
	virtual void updateForce(class Particle* p, float dt);
};

