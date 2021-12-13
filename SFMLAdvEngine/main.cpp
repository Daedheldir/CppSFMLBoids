#include "BoidsApp.h"
#include "BinTree.h"

int main() {

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