#pragma once
#include <iostream>

#include "BoidFlock.h"
#include "AlignmentBehaviour.h"
#include "CohesionBehaviour.h"
#include "SeparationBehaviour.h"
#include "RandomMovementBehaviour.h"

#include <SFML/Graphics.hpp>

namespace dh {
	//GAME DATA
	class GameData {
	public:
		bool bGameRunning = false;

		//views
		std::map<std::string, sf::View> viewsMap;

		//boid flocks
		BoidFlock boidFlock{ std::map<FlockBehaviourTypes, FlockBehaviour*>
		{
			{
				{ FlockBehaviourTypes::Alignment, new AlignmentBehaviour{ 1.0f } },
				{ FlockBehaviourTypes::Cohesion, new CohesionBehaviour{0.1f} },
				{ FlockBehaviourTypes::Separation, new SeparationBehaviour{1.5f} },
				{ FlockBehaviourTypes::RandomMovement, new RandomMovementBehaviour{0.5f} }
			}
		}
		};

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