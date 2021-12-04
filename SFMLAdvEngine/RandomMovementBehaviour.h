#pragma once
#include "FlockBehaviour.h"

class RandomMovementBehaviour : public FlockBehaviour {
public:
	RandomMovementBehaviour(float weight) : FlockBehaviour{ weight } {};
	sf::Vector2f operator() (const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView) const
	{
		sf::Vector2 randomMove{ 1 - (rand() % 200) / 100.0f,1 - (rand() % 200) / 100.0f };
		return randomMove * weight;
	}
};