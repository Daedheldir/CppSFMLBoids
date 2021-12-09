#pragma once
#include <SFML/Graphics.hpp>
#include "Quadtree.h"

class BoidAgentData
{
public:
	BoidAgentData();
	BoidAgentData(sf::Vector2f position);

	~BoidAgentData();
public:
	sf::Vector2f position;
	sf::Vector2f acceleration;
	dh::Quadtree<sf::Vector2f, BoidAgentData*>* currentNode;
};

