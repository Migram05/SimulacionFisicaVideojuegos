#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#define GRAVITY_VAL  -9.8;
class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel,Vector3 realV, Vector3 acc, float m, float d, float lT);
	~Particle();

	void integrate(double dt);
	bool checkAlive();
private:
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
	float gravity;
	float timeAlive = 0, lifeTime;
	const float dumping;
	physx::PxTransform pose;
	RenderItem* renderItem;
};

