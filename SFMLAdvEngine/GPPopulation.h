#pragma once
#include "BoidFlock.h"
#include "BinTree.h"
#include "FunctorBase.h"
#include "AlignmentBehaviour.h"
#include "CohesionBehaviour.h"
#include "SeparationBehaviour.h"
#include "RandomMovementBehaviour.h"

#include <vector>

//binary tree used to calculate color
class GPColor : public sf::Color {
	BinTree < std::unique_ptr<FunctorBase>> redTree;
	BinTree < std::unique_ptr<FunctorBase>> greenTree;
	BinTree < std::unique_ptr<FunctorBase>> blueTree;
};
class GPPopulation
{
public:
	GPPopulation(const size_t populationsSize, const size_t flockSize, const unsigned int iterationsToRun);

private:
	const unsigned int iterationsToRun;
	std::vector<BoidFlock> populations;
};

