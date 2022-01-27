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
	if (m_gameData->inputImage.loadFromFile("../Data/input_img_landscape.bmp"))
	{
		std::cout << "Successfuly loaded input img." << std::endl;
	}
	else {
		std::cout << "Couldn't load input img." << std::endl;
		return -1;
	}
	constexpr int BOID_POPULATIONS = 1;
	constexpr int BOID_POP_SIZE = 2000;
	constexpr int ITER_BETWEEN_EVALUATIONS = 500;
	constexpr int EVALUATIONS_COUNT = 250;
	constexpr int MAX_ITERATIONS = ITER_BETWEEN_EVALUATIONS * EVALUATIONS_COUNT;
	const unsigned int ITERATIONS_BETWEEN_IMAGE_SAVE = static_cast<unsigned int>(std::ceil(MAX_ITERATIONS / 998.0f));

	m_gameData->gpPopulationController = new GPPopulationController{ m_gameData->inputImage, BOID_POPULATIONS, BOID_POP_SIZE, ITER_BETWEEN_EVALUATIONS, ITERATIONS_BETWEEN_IMAGE_SAVE, MAX_ITERATIONS };
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