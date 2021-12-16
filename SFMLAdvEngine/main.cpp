#include "BoidsApp.h"
#include "BinTree.h"
#include "FunctorBase.h"
#include "AdditionFunctor.h"
#include "MultiplicationFunctor.h"
#include "DivisionFunctor.h"
#include <vector>

int main() {

	srand(0);	//seeding with 0 instead of time, so our simulation is deterministic

	dh::GameDataRef m_gameData = std::make_shared<dh::GameData>();
	if (m_gameData->inputImage.loadFromFile("../Data/input_img.bmp"))
	{
		std::cout << "Successfuly loaded input img." << std::endl;
	}
	constexpr int boidPopulations = 3;
	constexpr int boidPopSize = 2000;
	constexpr int iterationBetweenEvaluation = boidPopSize / 100;
	constexpr int totalIterations = iterationBetweenEvaluation * 100;
	m_gameData->gpPopulationController = new GPPopulationController{ m_gameData->inputImage, boidPopulations, boidPopSize, iterationBetweenEvaluation, totalIterations };
	BoidsApp boidsApp
	(
		m_gameData,
		{ m_gameData->inputImage.getSize().x, m_gameData->inputImage.getSize().y },
		"CPPBoids",
		240,
		30,
		false
	);
	boidsApp.Run();

	return 0;
}