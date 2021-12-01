
#include "SFMLGameEngine.h"
#include "BoidFlock.h"
#include "Definitions.h"

class BoidsApp : public dh::SFMLGameEngine {
public:
	BoidsApp(dh::GameDataRef m_gameData) : m_gameData(m_gameData), SFMLGameEngine(m_gameData, { dh::definitions::windowSizeX, dh::definitions::windowSizeY }, "test", 60, 60, false) {

	}
protected:
	dh::GameDataRef m_gameData;
	void loadResources() {};
	void handleEvents(sf::Event& ev) {
		if ((ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) || ev.type == sf::Event::Closed) 
			m_gameData->bGameRunning = false; 
	};
	void handleInput() {};
	void handleLogic() { flock.Update(); };
	void handleDrawing() 
	{ 
		getGraphics().getRenderWindow().clear(); 
		getGraphics().getRenderWindow().draw(flock.boidsVerticesArr); 
		getGraphics().getRenderWindow().display(); 
	};

private:
	BoidFlock flock;
	
};

int main() {
	srand(0);
	dh::GameDataRef m_gameData = std::make_shared<dh::GameData>();
	BoidsApp boidsApp(m_gameData);
	boidsApp.Run();
	return 0;
}