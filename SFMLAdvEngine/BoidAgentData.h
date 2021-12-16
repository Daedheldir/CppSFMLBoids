#pragma once
#include <SFML/Graphics.hpp>
#include "Quadtree.h"
class BoidAgentData
{
public:
	BoidAgentData();
	BoidAgentData(sf::Vector2f position, sf::Color color = sf::Color::Red);

	bool operator==(const BoidAgentData& other) const {
		return position == other.position
			&& acceleration == other.acceleration
			&& color == other.color
			&& leftColorsCounter == other.leftColorsCounter
			&& containingNode == other.containingNode;
	}

	~BoidAgentData();
public:
	Quadtree<BoidAgentData*>* containingNode;
	sf::Vector2f position;
	sf::Vector2f acceleration;
	sf::Color color;
	unsigned int leftColorsCounter = 0;
};

