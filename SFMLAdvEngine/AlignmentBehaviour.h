#pragma once
#include "FlockBehaviour.h"

class AlignmentBehaviour : public FlockBehaviour {
public:
	AlignmentBehaviour(float weight) : FlockBehaviour{ weight } {};
	sf::Vector2f operator() (const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView) const
	{
		if (boidsInView.size() <= 0) return currentBoid.acceleration;

		sf::Vector2f alignmentMove(0, 0);

		for (auto& heading : boidsInView) {
			alignmentMove += heading->acceleration;
		}

		alignmentMove *= 1.0f / boidsInView.size();

		return alignmentMove * weight;
	}
};