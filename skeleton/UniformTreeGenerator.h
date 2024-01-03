#pragma once
#include "UniformParticleGenerator.h"
class UniformTreeGenerator : public UniformParticleGenerator
{
private:
    float minSeparation;
    particleInfo treeModel;
public:
    UniformTreeGenerator(class ParticleSystem* parentSys, string name, Vector3 pos, float maxDim,float minSeparation, particleInfo model, int nP);
    ~UniformTreeGenerator();
    virtual list<Particle*> generateParticles() override;
};

