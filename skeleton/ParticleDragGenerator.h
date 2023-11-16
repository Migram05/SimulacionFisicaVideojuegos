#pragma once
#include "ForceGenerator.h"
class ParticleDragGenerator : public ForceGenerator
{
protected:
	float k1, k2;
public:
	ParticleDragGenerator(Vector3 o, float v1, float v2, float r = -1, bool oneT = false);
	virtual void updateForce(class Particle* p, float dt);
	inline void setDrag(float v1, float v2) { k1 = v1; k2 = v2; }
	inline float getK1() { return k1; }
	inline float getK2() { return k2; }
};

