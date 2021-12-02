#pragma once
#include <thread>
#include <functional>

#include "GameData.h"
#include "ResourceManager.h"

namespace dh {
	class GraphicsEngine
	{
	public:
		GraphicsEngine(dh::GameDataRef gameData, sf::Vector2u uWindowSize, std::string appName, bool bFullscreen, float fFpsLimit = 60);
		GraphicsEngine(dh::GameDataRef gameData, std::string appName, bool bFullscreen, float fFpsLimit = 60);

		~GraphicsEngine();

		void dispose();

		sf::RenderWindow& getRenderWindow();

		sf::Vector2u getWindowSize();

		dh::Resource_Manager<sf::Texture, std::string>& getTextureManager();
		dh::Resource_Manager<sf::Font, std::string>& getFontManager();


		void startRenderingThread(const std::function<void()>& drawFunc);
	private:
		void m_initialize(std::string appName, bool bFullscreen);

		void m_renderingThread(const std::function<void()>& drawFunc);

		void m_updateRenderingClock();
	private:
		sf::RenderWindow	m_renderWindow;
		sf::Vector2u		m_uWindowSize;

		dh::GameDataRef m_gameData;

		bool renderThreadRunning = false;
		std::thread m_renderThread;

		//resource management
		dh::Resource_Manager<sf::Texture, std::string>	m_textureManager;
		dh::Resource_Manager<sf::Font, std::string>		m_fontManager;

		//time related members
		float	m_fRenderElapsedTime;

		sf::Clock	m_renderClk;
		sf::Time	m_renderTime;

		float	m_fFPSLimit;				//max rendering updates per second equal to 1.0f / (int) maxFps

		float	m_fRenderCounter;
	};
}
