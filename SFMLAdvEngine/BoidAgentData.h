#pragma once
#include <SFML/Graphics.hpp>
class BoidAgentData
{
public:
	BoidAgentData();
	BoidAgentData(sf::Vector2f position);

	~BoidAgentData();
public:
	sf::Vector2f position;
	sf::Vector2f acceleration;
};

