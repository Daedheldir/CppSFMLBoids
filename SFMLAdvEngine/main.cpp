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
	BoidsApp boidsApp
	(
		m_gameData,
		{ dh::definitions::windowSizeX, dh::definitions::windowSizeY },
		"CPPBoids",
		60,
		60,
		false
	);
	boidsApp.Run();

	return 0;
}