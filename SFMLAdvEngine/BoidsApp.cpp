#include "BoidsApp.h"

BoidsApp::BoidsApp(dh::GameDataRef m_gameData, sf::Vector2u uWindowSize, std::string appName, unsigned int maxUpdatesPerSecond, uint8_t fpsLimit, bool fullscreen)
	: SFMLGameEngine(
		m_gameData,
		appName,
		maxUpdatesPerSecond,
		fpsLimit,
		fullscreen
	)
{
	m_gameData->FPSCounter.setFont(m_gameData->font);
	m_gameData->FPSCounter.setCharacterSize(15);
	//m_gameData->FPSCounter.setPosition(this->getGraphics().getRenderWindow().mapPixelToCoords(sf::Vector2i(5, 5)));
	m_gameData->FPSCounter.setPosition({ 0,0 });


	m_gameData->renderTexture.create(dh::definitions::windowSizeX, dh::definitions::windowSizeY);
	m_gameData->renderTextureRectShape.setSize({ dh::definitions::windowSizeX, dh::definitions::windowSizeY });
	m_gameData->renderTextureRectShape.setPosition(0, 0);
	m_gameData->renderTextureRectShape.setTexture(&m_gameData->renderTexture.getTexture());
}
void BoidsApp::LoadResources()
{
	if (m_gameData->font.loadFromFile("..\\Data\\Fonts\\arial.ttf"))
		std::cout << "Sucessfuly loaded font." << std::endl;
	else
		std::cout << "Couldn't load font!" << std::endl;
}

void BoidsApp::handleEvents(sf::Event& ev)
{
	if ((ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) || ev.type == sf::Event::Closed)
		this->dispose();
}
void BoidsApp::handleInput()
{
}
void BoidsApp::handleLogic()
{
	//update flock
	TextUpdate();
	m_gameData->boidFlock.Update();

}

void BoidsApp::handleDrawing()
{
	//update timing
	getGraphics().UpdateClock();

	//update render texture
	m_gameData->renderTexture.draw(m_gameData->boidFlock.boidsVerticesArr);
	m_gameData->renderTexture.display();

	//draw stuff
	getGraphics().getRenderWindow().clear();
	getGraphics().getRenderWindow().draw(m_gameData->renderTextureRectShape);

	std::unique_lock<std::mutex> fpsLock(lockFPSCounter);
	getGraphics().getRenderWindow().draw(this->m_gameData->FPSCounter);
	fpsLock.unlock();

	m_gameData->boidFlock.boidsQuadtree.draw(getGraphics().getRenderWindow());
	getGraphics().getRenderWindow().display();
}

void BoidsApp::TextUpdate()
{
	std::string str = "FPS: ";
	str += std::to_string(1.0f / getGraphics().GetElapsedTime());

	str += "\nUpdates per second: ";
	str += std::to_string(1.0f / getLogic().GetUpdateElapsedTime());

	std::unique_lock<std::mutex> fpsLock(lockFPSCounter);
	this->m_gameData->FPSCounter.setString(str);
	fpsLock.unlock();
}

