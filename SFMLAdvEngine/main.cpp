#include "BoidsApp.h"
#include "BinTree.h"
#include "FunctorBase.h"
#include "AdditionFunctor.h"
#include "MultiplicationFunctor.h"
#include "DivisionFunctor.h"
#include <vector>
#include "stdlib.h"
#include "TestBinTree.h"

int main() {
	//cout << "Prueba";

	//TestBinTree test;

	//test.testBinTree();

	//system("pause");
	//------------------------------------
	//----- SEEDS HALL OF FAME -----------
	//------------------------------------
	//316
	//128
	srand(3);	//seeding with constant instead of time, so our simulation is deterministic

	dh::GameDataRef m_gameData = std::make_shared<dh::GameData>();
	if (m_gameData->inputImage.loadFromFile("../Data/miniUs.png"))
	{
		std::cout << "Successfuly loaded input img." << std::endl;
	}
	else {
		std::cout << "Couldn't load input img." << std::endl;
		return -1;
	}
	constexpr int boidPopulations = 1;
	constexpr int boidPopSize = 2000;
	constexpr int iterationBetweenEvaluation = 500;
	constexpr int evaluationsCount = 250;
	constexpr int totalIterations = iterationBetweenEvaluation * evaluationsCount;
	const unsigned int iterationBetweenImageSave = static_cast<unsigned int>(std::ceil(totalIterations / 998.0f));

	m_gameData->gpPopulationController = new GPPopulationController{ m_gameData->inputImage, boidPopulations, boidPopSize, iterationBetweenEvaluation, iterationBetweenImageSave, totalIterations };
	BoidsApp boidsApp
	(
		m_gameData,
		{ m_gameData->inputImage.getSize().x, m_gameData->inputImage.getSize().y },
		"CPPBoids",
		240,	//updates per second
		30,		//fps
		false
	);
	boidsApp.Run();

	delete m_gameData->gpPopulationController;
	return 0;
}