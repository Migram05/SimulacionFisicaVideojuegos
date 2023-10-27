#pragma once
#include "ParticleGenerator.h"
#include <list>
#include "Particle.h"
#include <random>
class UniformParticleGenerator : public ParticleGenerator
{
private:
    Vector3 shape;
public:
    UniformParticleGenerator(class ParticleSystem* parentSys, string name, Vector3 pos,float maxDim, Vector3 shape, particleInfo model, int nP, bool oneT = false);
    ~UniformParticleGenerator();
    list<Particle*> generateParticles() override;

};

