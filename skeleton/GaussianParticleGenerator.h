#pragma once
#include "ParticleGenerator.h"
#include <list>
#include "Particle.h"
#include <random>
using namespace std;
class GaussianParticleGenerator : public ParticleGenerator
{

public:
    GaussianParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model);
    ~GaussianParticleGenerator();
    list<Particle*> generateParticles(int n) override;
};

