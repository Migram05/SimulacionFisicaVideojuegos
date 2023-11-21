#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator
{
protected:
	float K, dR;
	class Particle* otherParticle;
public:
	SpringForceGenerator(class Particle* p, float V, float d);
	void updateForce(class Particle* p, float dt) override;
	inline float getK() { return K; }
	inline void setK(float v) { K = v; }
};

