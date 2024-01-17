#include "UniformTreeGenerator.h"

UniformTreeGenerator::UniformTreeGenerator(ParticleSystem* parentSys, string name, Vector3 pos, float maxDim, float minSep, particleInfo model, int nP) : UniformParticleGenerator(parentSys, name, pos, maxDim, Vector3(1,0,1), model, nP, true)
{
	minSeparation = minSep;
	treeModel = { Vector3(0,1,0), Vector3(0,0,0), 0.98, -1, 1000,0, particleType::pT_custom,Vector4(0.58,0.29,0,1), CreateShape(physx::PxBoxGeometry(0.5, 2, 0.5)), false, 0 };
}

UniformTreeGenerator::~UniformTreeGenerator()
{
}

list<Particle*> UniformTreeGenerator::generateParticles()
{
	pL.clear();
	unsigned seed = (rand() % 1001) - 1000;
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<> distribution(minSeparation, maxDispersion);
	for (int i = 0; i < numParticles; ++i) {
		Vector3 offsetPos = Vector3(distribution(generator), 0, distribution(generator));
		pModel.origin = position + offsetPos + Vector3(0,2,0);
		treeModel.origin = position + offsetPos;
		float colorVar = rand() % 100;
		pModel.color = { 0, (colorVar+10) / 100, 0,1 };
		float sizeRand = rand() % 200;
		pModel.geometry = CreateShape(physx::PxSphereGeometry(1.5 + (sizeRand / 100)));
		Particle* p = new Particle(pModel, this);
		Particle* p2 = new Particle(treeModel, this);
		pL.push_back(p);
		pL.push_back(p2);
	}
	if (oneTime) shouldDestroyItself = true;
	return pL;
}
