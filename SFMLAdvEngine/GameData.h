#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

namespace dh {
	//GAME DATA
	class GameData {
	public:
		bool bGameRunning = false;

		//views
		std::map<std::string, sf::View> viewsMap;
	};

	//reference to game data;
	typedef std::shared_ptr<GameData> GameDataRef;
}