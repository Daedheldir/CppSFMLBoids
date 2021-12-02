#pragma once

#include "SFMLGameEngine.h"
#include "Definitions.h"

class BoidsApp : public dh::SFMLGameEngine {
public:
	BoidsApp(dh::GameDataRef m_gameData, sf::Vector2u uWindowSize, std::string appName, unsigned int maxUpdatesPerSecond, uint8_t fpsLimit, bool fullscreen);
protected:
	virtual void LoadResources();
	virtual void handleEvents(sf::Event& ev);	//has to run on main thread
	virtual void handleInput();					//has to run on main thread
	virtual void handleLogic();
	virtual void handleDrawing();				//if we are to use render texture this has to run on the main thread

	void TextUpdate();
private:
	std::mutex lockFPSCounter;
};