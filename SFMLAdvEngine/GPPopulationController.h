#pragma once
#include "BoidFlock.h"
#include "BinTree.h"
#include "FunctorBase.h"
#include "AlignmentBehaviour.h"
#include "CohesionBehaviour.h"
#include "SeparationBehaviour.h"
#include "RandomMovementBehaviour.h"

#include <vector>

class GPColor : public sf::Color {
public:
	GPColor() {};
	BinTree < std::unique_ptr<FunctorBase>> redTree;
	BinTree < std::unique_ptr<FunctorBase>> greenTree;
	BinTree < std::unique_ptr<FunctorBase>> blueTree;
};
class GPPopulationController
{
public:
	GPPopulationController(sf::Image& refImage, const size_t populationsSize, const size_t flockSize, const unsigned int iterationsToRun);
	~GPPopulationController();
	void CreatePopulations(const size_t populationsSize, const size_t flockSize);
	void UpdatePopulations();
	const BoidFlock& GetBestPopulation();
	sf::RenderTexture& GetBestPopulationRenderTexture();

public:
	unsigned int bestPopulationIndex = 0;
private:
	const unsigned int iterationsToRun;
	const sf::Image& refImage;

	std::vector<std::vector<GPColor>> populationColors;

	std::vector<std::vector<int>> populationBoidScores;
	std::vector<int> populationScores;

	std::vector<sf::Image> populationCanvases;
	std::vector<BoidFlock> populations;
};

