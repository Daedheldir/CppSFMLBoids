#pragma once
#include <iostream>

#include "BoidFlock.h"
#include "AlignmentBehaviour.h"
#include "CohesionBehaviour.h"
#include "SeparationBehaviour.h"
#include "RandomMovementBehaviour.h"
#include "GPPopulationController.h"
#include <SFML/Graphics.hpp>

namespace dh {
	//GAME DATA
	class GameData {
	public:
		bool bGameRunning = false;
		bool drawQuadtree = false;

		//views
		std::map<std::string, sf::View> viewsMap;

		//boid flocks
		GPPopulationController* gpPopulationController;

		//input image
		sf::Image inputImage;

		//render texture for drawing
		sf::RenderTexture	renderTexture;
		sf::RectangleShape	renderTextureRectShape;

		//statistics
		sf::Font	font;
		sf::Text	FPSCounter;
		float		fFramesPerSecond = 0.0f;
		float		fUpdatesPerSecond = 0.0f;
	};

	//reference to game data;
	typedef std::shared_ptr<GameData> GameDataRef;
}