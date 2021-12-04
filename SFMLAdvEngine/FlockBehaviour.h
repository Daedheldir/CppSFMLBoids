#pragma once
#include "BoidAgentData.h"
#include <SFML/System/Vector2.hpp>

class FlockBehaviour
{
public:
	FlockBehaviour(float weight) : weight{ weight } {};
	virtual sf::Vector2f operator () (const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView) const = 0;

public:
	const float weight;
};

