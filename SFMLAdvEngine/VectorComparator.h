#pragma once
#include <SFML/System/Vector2.hpp>
#include "MathAdditions.h"

class VectorComparator {
public:
	bool operator() (const sf::Vector2f& a, const sf::Vector2f& b) const
	{
		float mag_a = mathAdditions::VectorSqrMagnitude(a);
		float mag_b = mathAdditions::VectorSqrMagnitude(b);
		return  mag_a < mag_b;
	};
};