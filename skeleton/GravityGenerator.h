#pragma once
#include "ForceGenerator.h"
class GravityGenerator :public ForceGenerator
{
protected:
	Vector3 gravity = { 0,-9.8,0 };
public:
	GravityGenerator(Vector3 o,const Vector3& g = {0,-9.8,0}, float r = -1, bool oneT = false);
	virtual void updateForce(class Particle* p, float dt) override;
	inline void setGravity(Vector3 v) { gravity = v; }
	inline Vector3 getGravity() { return gravity; }
};

