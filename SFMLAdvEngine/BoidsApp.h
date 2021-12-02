#pragma once

#include "SFMLGameEngine.h"
#include "Definitions.h"

class BoidsApp : public dh::SFMLGameEngine {
public:
	BoidsApp(dh::GameDataRef m_gameData);

protected:
	virtual void loadResources();
	virtual void handleEvents(sf::Event& ev);	//has to run on main thread
	virtual void handleInput();					//has to run on main thread
	virtual void handleLogic();
	virtual void handleDrawing();				//if we are to use render texture this has to run on the main thread
};