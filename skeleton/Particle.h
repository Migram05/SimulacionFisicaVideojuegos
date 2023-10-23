#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#define GRAVITY_VAL  -9.8;

enum particleType {
	pT_custom, pT_Bullet, pT_Cannon, pT_Spark
};

struct particleInfo
{
	Vector3 origin;
	Vector3 velocity;
	Vector3 acceleration;
	float dumping;
	float lifeTime;
	float maxDistance;
	particleType type;
	Vector4 color;
	physx::PxShape* geometry;
};

class Particle
{
public:
	Particle(particleInfo pI, class ParticleGenerator* = nullptr);
	virtual ~Particle();
	void integrate(double dt);
	bool checkAlive();
protected:
	void setParticleValues(const particleInfo i);
	Vector3 velocity;
	Vector3 origin;
	Vector3 realVelocity;
	Vector3 acceleration;
	float mass, realMass;
	float gravity;
	float maxDistance;
	float timeAlive = 0, lifeTime;
	const float dumping;
	physx::PxTransform pose;
	physx::PxShape* shape;
	RenderItem* renderItem;
	class ParticleGenerator* generator;
};

class Firework : public Particle 
{
public:
	Firework(particleInfo pI, class ParticleGenerator*);
	~Firework();
};

