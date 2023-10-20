#pragma once
#include "ParticleGenerator.h"
#include <list>
#include "Particle.h"
#include <random>
using namespace std;
class GaussianParticleGenerator : public ParticleGenerator
{

public:
    GaussianParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model, int nParticles, bool oneT = false);
    ~GaussianParticleGenerator();
    list<Particle*> generateParticles() override;
};

