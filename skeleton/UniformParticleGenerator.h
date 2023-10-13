#pragma once
#include "ParticleGenerator.h"
#include <list>
#include "Particle.h"
#include <random>
class UniformParticleGenerator : public ParticleGenerator
{
public:
    UniformParticleGenerator(string name, Vector3 pos, Vector3 vel, particleInfo model);
    ~UniformParticleGenerator();
    list<Particle*> generateParticles(int n) override;

};

