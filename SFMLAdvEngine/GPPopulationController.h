#pragma once
#include "BoidFlock.h"
#include "BinTree.h"
#include "FunctorBase.h"
#include "AlignmentBehaviour.h"
#include "CohesionBehaviour.h"
#include "SeparationBehaviour.h"
#include "RandomMovementBehaviour.h"

#include <vector>

/*
1. Bit - wise AND : X & Y(or R)
2. Bit - wise OR : X | Y(or R)
3. Bit - wise XOR : X ^ Y(or R)
4. NOT : !X
5. Left - shift : X << 2 (multiplied by 2)
6. Right - shift : X >> 2 (divided by 2)
7. ADD : X + Y(or R)
8. SUB : X – Y(or R)
9. MUL : X * Y(or R)
10. DIV : X / Y(or R) if Y > 0
11. SIN: sin(X) * 255
12. COS : cos(X) * 255
13. TAN : tan(X) * 255
*/
class GPColor : protected sf::Color {
public:
	GPColor() : r{ static_cast<float>(1 + rand() % 255) } {};
	sf::Color GetColor(sf::Vector2f position) {
		posX = position.x;
		posY = position.y;

		uint8_t red = static_cast<uint8_t>(static_cast<int>(posX - posY + r) % 256);
		uint8_t green = static_cast<uint8_t>(static_cast<int>(posX + posY * r) % 256);
		uint8_t blue = static_cast<uint8_t>(static_cast<int>(posX / posY + r) % 256);

		return sf::Color{ red, green, blue };
	}
	float posX = 0;
	float posY = 0;
	float r = 0;
	//BinTree < std::shared_ptr<FunctorBase>> redTree;
	//BinTree < std::shared_ptr<FunctorBase>> greenTree;
	//BinTree < std::shared_ptr<FunctorBase>> blueTree;
};
class GPPopulationController
{
public:
	GPPopulationController(sf::Image& refImage, const size_t populationsSize, const size_t flockSize, const unsigned int iterationsBetweenEvaluation, const unsigned int totalIterations);
	~GPPopulationController();
	void CreatePopulations(const size_t populationsSize, const size_t flockSize);
	void UpdateGP();
	const BoidFlock& GetBestPopulation() const;

private:
	void UpdatePopulations();
	void EvaluatePopulations();
public:
	bool simRunning = true;
private:
	unsigned int bestPopulationIndex = 0;
	unsigned int iterationsCounter = 0;
	unsigned int evaluationsCounter = 0;

	const unsigned int iterationsBetweenEvaluation;
	const unsigned int totalIterations;

	const sf::Image& refImage;

	//boid related vecs
	std::vector<std::vector<GPColor>> populationColors;
	const float boidDiscardPercentage = 0.5f; //discarding 20% of worst boids
	std::vector<std::vector<std::pair<unsigned int, int>>> populationBoidScores;	//flocks -> boids -> pair of boid index in vector and boid score

	//population related vecs
	std::vector<int> populationScores;
	std::vector<sf::Image> populationCanvases;
	std::vector<BoidFlock> populations;
};

