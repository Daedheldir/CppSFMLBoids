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

			if (mathAdditions::VectorSqrMagnitude(neighbourPos->position - currentBoid.position) < BoidFlock::SQUARE_NEIGHBOUR_AVOIDANCE_RADIUS)
			{
				nAvoid++;
				avoidanceMove += currentBoid.position - neighbourPos->position;
			}
		}

		if (nAvoid > 0)
		{
			avoidanceMove *= 1.0f / (float)nAvoid;
		}

		return avoidanceMove * weight;
	}
};