#pragma once

#include <chrono>

#include <SFML/Graphics.hpp>
#include <memory>
#include <mutex>
#include <iostream>

#include "GameData.h"

#include "LogicEngine.h"
#include "GraphicsEngine.h"

#include "ResourceManager.h"

namespace dh {


	//reference to game data;
	typedef std::shared_ptr<GameData> GameDataRef;

	class SFMLGameEngine
	{
	public:
		SFMLGameEngine(GameDataRef m_gameData, sf::Vector2u uWindowSize, std::string appName, unsigned int maxUpdatesPerSecond, uint8_t fpsLimit, bool fullscreen = false);
		SFMLGameEngine(GameDataRef m_gameData, std::string appName, unsigned int maxUpdatesPerSecond, uint8_t fpsLimit, bool fullscreen = false);

		virtual ~SFMLGameEngine();

		sf::Vector2u getWindowSize();

		dh::GraphicsEngine & getGraphics();

		bool isGameRunning();

		void Run();

	protected:
		virtual void loadResources() = 0;
		virtual void handleEvents(sf::Event & ev) = 0;
		virtual void handleInput() = 0;
		virtual void handleLogic() = 0;
		virtual void handleDrawing() = 0;
	private:

		void m_pollEvents();

	protected:
		GameDataRef m_gameData;
		float		fEventsElapsedTime;

	private:
		GraphicsEngine	graphics;
		LogicEngine		logic;
		
		//clocks
		sf::Clock	eventsClk;
		sf::Time	eventsTime;
	};
}