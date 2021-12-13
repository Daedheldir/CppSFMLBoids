#include "BoidAgentData.h"


BoidAgentData::BoidAgentData(sf::Vector2f position, sf::Color color) :
	position{ position },
	acceleration{ 1 - (rand() % 200) / 100.0f,1 - (rand() % 200) / 100.0f },
	color{ color }
{
}

BoidAgentData::BoidAgentData() :
	position{ 0,0 },
	acceleration{ 1 - (rand() % 200) / 100.0f,1 - (rand() % 200) / 100.0f },
	color{ rand() % 255, rand() % 255, rand() % 255 }
{
}

BoidAgentData::~BoidAgentData()
{
}
