#pragma once
#include "FlockBehaviour.h"
#include "MathAdditions.h"

class SeparationBehaviour : public FlockBehaviour {
public:
	SeparationBehaviour(float weight) : FlockBehaviour{ weight } {};
	sf::Vector2f operator() (const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView) const
	{
		if (boidsInView.size() <= 0) return sf::Vector2f(0, 0);

		//add all points together and average
		sf::Vector2f avoidanceMove(0, 0);
		int nAvoid = 0;

		sf::Vector2f closestAgentPos = boidsInView[0]->position;

		for (auto& neighbourPos : boidsInView)
		{
			sf::Vector2f relativeVec = currentBoid.position - neighbourPos->position;

			if (std::abs(relativeVec.x) > BoidFlock::SQUARE_BOIDS_VIEW_RANGE
				|| std::abs(relativeVec.y) > BoidFlock::SQUARE_BOIDS_VIEW_RANGE)
			{
				continue;
			}

			if (mathAdditions::VectorSqrMagnitude(relativeVec) < BoidFlock::SQUARE_NEIGHBOUR_AVOIDANCE_RADIUS)
			{
				nAvoid++;
				avoidanceMove += relativeVec;
			}
		}

		if (nAvoid > 0)
		{
			avoidanceMove *= 1.0f / (float)nAvoid;
		}

		return avoidanceMove * weight;
	}
};