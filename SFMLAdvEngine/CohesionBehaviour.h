#pragma once
#include "FlockBehaviour.h"

class CohesionBehaviour : public FlockBehaviour {
public:
	CohesionBehaviour(float weight) : FlockBehaviour{ weight } {};
	sf::Vector2f operator() (const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView) const
	{
		if (boidsInView.size() <= 0) return sf::Vector2f(0, 0);

		sf::Vector2f cohesionMove(0, 0);

		//add all points together and average
		for (auto& pos : boidsInView) {
			cohesionMove += pos->position;
		}
		cohesionMove *= 1.0f / boidsInView.size();

		//create offset from agent position
		cohesionMove -= currentBoid.position;

		return cohesionMove * weight;
	}
};