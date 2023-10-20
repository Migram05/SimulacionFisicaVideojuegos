#pragma once
#include "ParticleGenerator.h"
#include <list>
#include "Particle.h"
#include <random>
class UniformParticleGenerator : public ParticleGenerator
{
private:
    int maxDimension;
    Vector3 shape;
public:
    UniformParticleGenerator(string name, Vector3 pos, Vector3 vel, int maxDim, Vector3 shape, particleInfo model, int nP, bool oneT = false);
    ~UniformParticleGenerator();
    list<Particle*> generateParticles() override;

};

