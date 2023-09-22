#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel);
	~Particle();

	void integrate(double dt);

private:
	Vector3 velocity;
	physx::PxTransform pose;
	RenderItem* renderItem;
};

