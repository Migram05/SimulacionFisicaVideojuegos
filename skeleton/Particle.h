#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#define GRAVITY_VAL  -9.8;

enum particleType {
	pT_Bullet, pT_Cannon
};

struct particleInfo
{
	Vector3 origin;
	Vector3 velocity;
	Vector3 acceleration;
	float dumping;
	float lifeTime;
	particleType type;
};

class Particle
{
public:
	Particle(particleInfo pI);
	~Particle();
	void integrate(double dt);
	bool checkAlive();
private:
	void setParticleValues(const particleInfo i);
	Vector3 velocity;
	Vector3 realVelocity;
	Vector3 acceleration;
	float mass, realMass;
	float gravity;
	float timeAlive = 0, lifeTime;
	const float dumping;
	physx::PxTransform pose;
	physx::PxShape* shape;
	RenderItem* renderItem;
};

