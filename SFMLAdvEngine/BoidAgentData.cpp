#include "BoidAgentData.h"


BoidAgentData::BoidAgentData(sf::Vector2f position) :
	position{ position },
	acceleration{ 1 - (rand() % 200) / 100.0f,1 - (rand() % 200) / 100.0f },
	currentNode{ nullptr }
{
}

BoidAgentData::BoidAgentData() :
	position{ 0,0 },
	acceleration{ 1 - (rand() % 200) / 100.0f,1 - (rand() % 200) / 100.0f },
	currentNode{ nullptr }
{
}

BoidAgentData::~BoidAgentData()
{
}
