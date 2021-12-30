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

	cout << "Prueba";

	TestBinTree test;

	test.testBinTree();

	system("pause");

	srand(0);	//seeding with 0 instead of time, so our simulation is deterministic

	dh::GameDataRef m_gameData = std::make_shared<dh::GameData>();
	if (m_gameData->inputImage.loadFromFile("../Data/input_img.bmp"))
	{
		std::cout << "Successfuly loaded input img." << std::endl;
	}
	constexpr int boidPopulations = 1;
	constexpr int boidPopSize = 2500;
	constexpr int iterationBetweenEvaluation = 100;
	constexpr int totalIterations = iterationBetweenEvaluation * 500;
	m_gameData->gpPopulationController = new GPPopulationController{ m_gameData->inputImage, boidPopulations, boidPopSize, iterationBetweenEvaluation, totalIterations };
	BoidsApp boidsApp
	(
		m_gameData,
		{ m_gameData->inputImage.getSize().x, m_gameData->inputImage.getSize().y },
		"CPPBoids",
		60,
		60,
		false
	);
	boidsApp.Run();

	delete m_gameData->gpPopulationController;
	return 0;
}