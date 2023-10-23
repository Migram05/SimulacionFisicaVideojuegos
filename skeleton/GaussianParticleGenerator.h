#pragma once
#include "ParticleGenerator.h"
#include <list>
#include "Particle.h"
#include <random>
using namespace std;
class GaussianParticleGenerator : public ParticleGenerator
{
private:
    float maxDimension;
public:
    GaussianParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model, float maxDim, int nParticles, bool oneT = false);
    ~GaussianParticleGenerator();
    void generateParticles() override;
};

