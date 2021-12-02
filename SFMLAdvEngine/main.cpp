#include "BoidsApp.h"

int main() {
	srand(0);
	dh::GameDataRef m_gameData = std::make_shared<dh::GameData>();
	BoidsApp boidsApp(m_gameData);
	boidsApp.Run();
	return 0;
}