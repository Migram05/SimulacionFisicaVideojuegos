#pragma once
#include "ForceGenerator.h"
#include "core.hpp"
class GravityGenerator :public ForceGenerator
{
protected:
	Vector3 gravity = { 0,-9.8,0 };
public:
	GravityGenerator(const Vector3& g = {0,-9.8,0});
	virtual void updateForce(class Particle* p) override;
	inline void setGravity(Vector3 v) { gravity = v; }
	inline Vector3 getGravity() { return gravity; }
};

