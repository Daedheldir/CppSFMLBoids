#pragma once
#include "BoidAgentData.h"
class BoidAgentController
{
	private:
	BoidAgentController();
	
public:
	static void MoveBoid(BoidAgentData* boidData, sf::Vector2f velocity);
};

