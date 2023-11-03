#pragma once
#include "ForceGenerator.h"
class GravityGenerator :public ForceGenerator
{
private:
	float gravity = -9.8;
public:
	virtual void updateForce(class Particle* p) override;
	inline void setGravity(float v) { gravity = v; }
	inline float getGravity() { return gravity; }
};

