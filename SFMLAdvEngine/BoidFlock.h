#pragma once
#include <array>
#include <map>
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>
#include "BoidAgentData.h"
#include "FlockBehaviour.h"
#include "Quadtree.h"
#include "MathAdditions.h"

enum class FlockBehaviourTypes {
	Alignment,
	Cohesion,
	Separation,
	RandomMovement
};
enum class NeighbourFindingMethod {
	BruteForce,
	Quadtree
};
class BoidFlock
{
public:
	BoidFlock(const BoidFlock& other);
	BoidFlock(size_t flockSize, std::map<FlockBehaviourTypes, std::shared_ptr<FlockBehaviour>> flockRules, sf::Vector2u boidPositionBound);
	BoidFlock& operator = (BoidFlock other);
	~BoidFlock() = default;
	void MoveBoids();

private:
	sf::Vector2f CalculateBoidMovement(const BoidAgentData& boid);

	template<NeighbourFindingMethod T>
	void GetBoidsInView(const BoidAgentData& boid, std::vector<BoidAgentData*>& boidsInView) {
		return std::vector<BoidAgentData*>();
	};

	template<>
	void GetBoidsInView<NeighbourFindingMethod::BruteForce>(const BoidAgentData& boid, std::vector<BoidAgentData*>& boidsInView) {
		boidsInView.reserve(std::round(boidsDataArr.size() * 0.8));

		for (auto& neighbour : boidsDataArr) {
			if (&neighbour == &boid) {
				continue;
			}
			sf::Vector2f relativeVec = boid.position - neighbour.position;

			if (std::abs(relativeVec.x) > BoidFlock::SQUARE_BOIDS_VIEW_RANGE
				|| std::abs(relativeVec.y) > BoidFlock::SQUARE_BOIDS_VIEW_RANGE)
			{
				continue;
			}

			if (mathAdditions::VectorSqrMagnitude(relativeVec) < BoidFlock::SQUARE_BOIDS_VIEW_RANGE)
			{
				boidsInView.emplace_back(&neighbour);
			}

		}
	};

	template<>
	void GetBoidsInView<NeighbourFindingMethod::Quadtree>(const BoidAgentData& boid, std::vector<BoidAgentData*>& boidsInView) {
		boidsInView.reserve(std::round(boidsDataArr.size() * 0.8));
		boidsQuadtree.queryRange(boid.position, { sqrt(SQUARE_BOIDS_VIEW_RANGE), sqrt(SQUARE_BOIDS_VIEW_RANGE) }, boidsInView);
	};
public:
	//parameters
	static const float SQUARE_NEIGHBOUR_AVOIDANCE_RADIUS;
	static const float SQUARE_BOIDS_VIEW_RANGE;
	static const float MAX_SPEED;
	static const float MAX_ACCELERATION_CHANGE; //range between 0-1f

	sf::Vector2u boidPosBounds;

	//variables
	std::vector<BoidAgentData> boidsDataArr;
	Quadtree<BoidAgentData*> boidsQuadtree;
	struct {
		float fTreeUpdateInterval = 5.0f;
		float fTreeUpdateTimer = 0.0f;
		bool bRebuildTree = false;
	}UpdateTimer;
	std::map<FlockBehaviourTypes, std::shared_ptr<FlockBehaviour>> flockBehaviours;
	sf::VertexArray boidsVerticesArr;
};


