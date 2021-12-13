#include "BoidsApp.h"
#include "BinTree.h"
#include "FunctorBase.h"
#include "AdditionFunctor.h"
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

	//we need to get something like that working
	//give me a sec
	std::vector<FunctorBase*> vec;
	vec.push_back(new AdditionFunctor(10));
	vec.push_back(new AdditionFunctor(5));

	int i = 0;
	for (auto& func : vec) {
		i += (*func)(i);
	}

	std::cout << i << std::endl;
	return 0;
}