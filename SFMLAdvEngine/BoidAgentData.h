#pragma once
#include <SFML/Graphics.hpp>

class BoidAgentData
{
public:
	BoidAgentData();
	BoidAgentData(sf::Vector2f position, sf::Color color = sf::Color::Red);

	/*float getx() const { return position.x };
	void setx(float val){ position.x=val};
	float gety() const { return position.y };
	void sety(float val){ position.y=val};//*/

	~BoidAgentData();
public:
	sf::Vector2f position;
	sf::Vector2f acceleration;
	sf::Color color;
	unsigned int leftColorsCounter = 0;
};

