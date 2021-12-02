
#include "SFMLGameEngine.h"
#include "Definitions.h"

class BoidsApp : public dh::SFMLGameEngine {
public:
	BoidsApp(dh::GameDataRef m_gameData) : SFMLGameEngine(m_gameData, { dh::definitions::windowSizeX, dh::definitions::windowSizeY }, "test", 60, 60, false) {
		m_gameData->renderTexture.create(dh::definitions::windowSizeX, dh::definitions::windowSizeY);
		m_gameData->renderTextureRectShape.setSize({ dh::definitions::windowSizeX, dh::definitions::windowSizeY });
		m_gameData->renderTextureRectShape.setPosition(0, 0);
		m_gameData->renderTextureRectShape.setTexture(&m_gameData->renderTexture.getTexture());
	};

protected:
	virtual void loadResources() {};
	virtual void handleEvents(sf::Event& ev) {
		if ((ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) || ev.type == sf::Event::Closed)
			this->dispose();
	};
	virtual void handleInput() {};
	virtual void handleLogic()
	{
		//update flock
		m_gameData->boidFlock.Update();

	};
	virtual void handleDrawing()
	{
		////update render texture
		//m_gameData->renderTexture.draw(m_gameData->boidFlock.boidsVerticesArr);
		//m_gameData->renderTexture.display();

		//getGraphics().getRenderWindow().clear();
		//getGraphics().getRenderWindow().draw(m_gameData->renderTextureRectShape);
		getGraphics().getRenderWindow().draw(m_gameData->boidFlock.boidsVerticesArr);
		getGraphics().getRenderWindow().display();
	};
};

int main() {
	srand(0);
	dh::GameDataRef m_gameData = std::make_shared<dh::GameData>();
	BoidsApp boidsApp(m_gameData);
	boidsApp.Run();
	return 0;
}