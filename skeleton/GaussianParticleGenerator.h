#pragma once
#include "ParticleGenerator.h"
#include <list>
#include "Particle.h"
#include <random>
using namespace std;
class GaussianParticleGenerator : public ParticleGenerator
{
private:

public:
    GaussianParticleGenerator(class ParticleSystem* parentSys, string name, Vector3 pos, particleInfo model, float maxDim, int nParticles, bool oneT = false);
    ~GaussianParticleGenerator();
    list<Particle*> generateParticles() override;
};

