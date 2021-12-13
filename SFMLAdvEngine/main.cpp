#include "BoidsApp.h"
#include "BinTree.h"
#include "FunctorBase.h"
#include "AdditionFunctor.h"
#include "MultiplicationFunctor.h"
#include "DivisionFunctor.h"
#include <vector>

int main() {

	srand(0);	//seeding with 0 instead of time, so our simulation is deterministic

	/*dh::GameDataRef m_gameData = std::make_shared<dh::GameData>();
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
	*/

	BinTree<FunctorBase*> bTree;

	bTree.insert()
		//we need to get something like that working
		//give me a sec

		std::cout << i << std::endl;
	return 0;
}