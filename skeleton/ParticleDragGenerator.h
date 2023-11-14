#pragma once
#include "ForceGenerator.h"
#include "core.hpp"
class ParticleDragGenerator : public ForceGenerator
{
protected:
	float k1, k2;
public:
	ParticleDragGenerator(float v1, float v2);
	virtual void updateForce(class Particle* p) override;
	inline void setDrag(float v1, float v2) { k1 = v1; k2 = v2; }
	inline float getK1() { return k1; }
	inline float getK2() { return k2; }
};

