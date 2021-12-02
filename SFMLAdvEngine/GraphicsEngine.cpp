#include "GraphicsEngine.h"


dh::GraphicsEngine::GraphicsEngine(dh::GameDataRef gameData, sf::Vector2u uWindowSize, std::string appName, bool bFullscreen, float fFpsLimit) :
	m_gameData(gameData),
	m_uWindowSize(uWindowSize),
	m_fFPSLimit(fFpsLimit)
{
	m_initialize(appName, bFullscreen);
}

dh::GraphicsEngine::GraphicsEngine(dh::GameDataRef gameData, std::string appName, bool bFullscreen, float fFpsLimit) :
	m_gameData(gameData),
	m_uWindowSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
	m_fFPSLimit(fFpsLimit)

{
	m_initialize(appName, bFullscreen);
}

dh::GraphicsEngine::~GraphicsEngine()
{
	if (renderThreadRunning)
		dispose();
}

void dh::GraphicsEngine::dispose()
{
	renderThreadRunning = false;
	if (m_renderThread.joinable()) {
		m_renderThread.join();
		std::cout << "Graphics thread joined." << std::endl;
	}
	else {
		std::cout << "Graphics thread not joinable!" << std::endl;
	}

	m_renderWindow.close();
}

sf::RenderWindow& dh::GraphicsEngine::getRenderWindow()
{
	return m_renderWindow;
}

sf::Vector2u dh::GraphicsEngine::getWindowSize()
{
	return m_uWindowSize;
}

dh::Resource_Manager<sf::Texture, std::string>& dh::GraphicsEngine::getTextureManager()
{
	return m_textureManager;
}

dh::Resource_Manager<sf::Font, std::string>& dh::GraphicsEngine::getFontManager()
{
	return m_fontManager;
}


void dh::GraphicsEngine::startRenderingThread(const std::function<void()>& drawFunc)
{
	renderThreadRunning = true;
	m_renderThread = std::thread(&dh::GraphicsEngine::m_renderingThread, this, drawFunc);
}

void dh::GraphicsEngine::m_initialize(std::string appName, bool bFullscreen)
{
	//creating render window 
	std::cout << "Creating window..." << std::endl;

	if (bFullscreen)
		this->m_renderWindow.create(sf::VideoMode::getDesktopMode(), appName, sf::Style::Fullscreen);
	else
		this->m_renderWindow.create(sf::VideoMode(m_uWindowSize.x, m_uWindowSize.y), appName, sf::Style::Close | sf::Style::Titlebar);

	std::cout << "\tWindow created.\n" << std::endl;

	//setting up views
	std::cout << "Setting up views..." << std::endl;
	for (auto& view : this->m_gameData->viewsMap) {
		view.second.setSize(static_cast<float>(m_uWindowSize.x), static_cast<float>(m_uWindowSize.y));
		view.second.setCenter(static_cast<float>(m_uWindowSize.x / 2), static_cast<float>(m_uWindowSize.y / 2));
	}

	std::cout << "\tViews initialized.\n" << std::endl;

}

void dh::GraphicsEngine::m_renderingThread(const std::function<void()>& drawFunc)
{
	std::cout << "\tRendering thread started." << std::endl;

	while (this->renderThreadRunning)
	{
		m_updateRenderingClock();
		drawFunc();
	}
}

void dh::GraphicsEngine::m_updateRenderingClock()
{
	double renderingTimeDiff = 0.0;

	//elapsed time update
	this->m_renderTime = this->m_renderClk.restart();
	this->m_fRenderElapsedTime = this->m_renderTime.asSeconds();

	m_fRenderCounter += m_fRenderElapsedTime;

	if (this->m_fRenderCounter >= 0.2f) {
		this->m_fRenderCounter -= 0.2f;
	}

	if (this->m_fRenderElapsedTime >= 0.5f) {
		this->m_fRenderElapsedTime = 0.5f;
	}

	//lock the updates per second
	if (this->m_fRenderElapsedTime < m_fFPSLimit) {

		renderingTimeDiff = m_fFPSLimit - this->m_fRenderElapsedTime;

		this->m_fRenderElapsedTime = static_cast<float>(m_fFPSLimit);
		std::this_thread::sleep_for(std::chrono::duration<float>(renderingTimeDiff));
	}

}
