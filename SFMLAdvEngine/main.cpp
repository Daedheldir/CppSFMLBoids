#include "BoidsApp.h"
#include <cstdlib>
#include "TestBinTree.h"

int main() {

	//BinTree test
	TestBinTree Test;

	Test.testBinTree();

	system("pause");

	//Boids:

	srand(0);
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