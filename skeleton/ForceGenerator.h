#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
class ForceGenerator
{
protected:
	float radius;
	bool oneTimeEffect, effectDone = false;
	RenderItem* renderItem;
	physx::PxTransform origin;
public:
	ForceGenerator(Vector3 o, float r = -1, bool oT = false);
	virtual void updateForce(class Particle* p) = 0;
	bool shouldDestroy() { return (oneTimeEffect && effectDone); }
	void setEffectDone() { effectDone = true; }
	bool isOneTimeEffect() { return oneTimeEffect; }
	~ForceGenerator();
};

