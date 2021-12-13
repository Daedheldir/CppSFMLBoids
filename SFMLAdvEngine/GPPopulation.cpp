#include "GPPopulation.h"

GPPopulation::GPPopulation(const size_t populationsSize, const size_t flockSize, const unsigned int runningIterations) : iterationsToRun(iterationsToRun)
{
	populations.reserve(populationsSize);
	//populationTrees.resize(populationsSize);

	for (int i = 0; i < populationsSize; ++i) {
		//boid flocks
		BoidFlock boidFlock{ flockSize, std::map<FlockBehaviourTypes, FlockBehaviour*>
		{
			{
				{ FlockBehaviourTypes::Alignment, new AlignmentBehaviour{ 1.0f } },
				{ FlockBehaviourTypes::Cohesion, new CohesionBehaviour{0.1f} },
				{ FlockBehaviourTypes::Separation, new SeparationBehaviour{1.5f} },
				{ FlockBehaviourTypes::RandomMovement, new RandomMovementBehaviour{0.5f} }
			}
		}
		};

		populations.push_back(boidFlock);

		/*
		//population trees
		populationTrees[i].reserve(flockSize);
		BinTree<std::unique_ptr<FunctorBase>> defaultTree;

		for (int j = 0; j < flockSize; ++j) {
			populationTrees[i].push_back(defaultTree);
		}*/
	}
}
