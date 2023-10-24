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
	Vector3 acceleration = { 0,1,0 };
	float dumping = 0.98;
	float lifeTime = 2;
	float maxDistance;
	particleType type;
	Vector4 color;
	physx::PxShape* geometry;
	bool destroySpawn = false;
	int destroySpawnNum;
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
	bool generateOnDestroy;
	particleInfo pInfoCopy;
	int generateNum;
	const float dumping;
	physx::PxTransform pose;
	physx::PxShape* shape;
	RenderItem* renderItem;
	class ParticleGenerator* generator;
};

class Firework : public Particle 
{
private:
	int type;
public:
	Firework(particleInfo pI, class ParticleGenerator*, int type = 1);
	~Firework();
};

