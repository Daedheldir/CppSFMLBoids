#pragma once
#include <array>
#include <map>
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>
#include "BoidAgentData.h"
#include "FlockBehaviour.h"
#include "Quadtree.h"
#include "MathAdditions.h"
#include "Definitions.h"

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
	BoidFlock(std::map<FlockBehaviourTypes, FlockBehaviour*> flockRules);
	~BoidFlock();
	void Update();

private:
	template<NeighbourFindingMethod T>
	std::vector<BoidAgentData*> GetBoidsInView(const BoidAgentData& boid) {
		return std::vector<BoidAgentData*>();
	};

	template<>
	std::vector<BoidAgentData*> GetBoidsInView<NeighbourFindingMethod::BruteForce>(const BoidAgentData& boid) {
		std::vector<BoidAgentData*> boidsInView;
		for (auto& neighbour : boidsDataArr) {
			if (&neighbour != &boid && mathAdditions::VectorSqrMagnitude(boid.position - neighbour.position) < BoidFlock::SQUARE_BOIDS_VIEW_RANGE) {
				boidsInView.push_back(&neighbour);
			}
		}
		return boidsInView;
	};

	template<>
	std::vector<BoidAgentData*> GetBoidsInView<NeighbourFindingMethod::Quadtree>(const BoidAgentData& boid) {
		std::list<BoidAgentData*> boidsInView = boidsQuadtree.queryRange(boid.position, { sqrt(SQUARE_BOIDS_VIEW_RANGE),sqrt(SQUARE_BOIDS_VIEW_RANGE) });
		std::vector<BoidAgentData*> output;
		output.reserve(boidsInView.size());
		for (auto& boid : boidsInView) {
			output.push_back(boid);
		}

		return output;
	};

	sf::Vector2f CalculateAlignment(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView);
	sf::Vector2f CalculateCohesion(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView);
	sf::Vector2f CalculateSeparation(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView);
	sf::Vector2f CalculateRandomMovement();
public:
	//parameters
	static constexpr unsigned int BOIDS_COUNT = 2000u;
	static constexpr float SQUARE_NEIGHBOUR_AVOIDANCE_RADIUS = 10.0f;
	static constexpr float SQUARE_BOIDS_VIEW_RANGE = 400.0f;
	static constexpr float MAX_SPEED = 0.5f;
	static constexpr float MAX_ACCELERATION_CHANGE = 0.05f; //range between 0-1f

	//variables
	std::array<BoidAgentData, BOIDS_COUNT> boidsDataArr;
	dh::Quadtree<sf::Vector2f, BoidAgentData*> boidsQuadtree;
	std::map<FlockBehaviourTypes, FlockBehaviour*> flockBehaviours;
	sf::VertexArray boidsVerticesArr;
};


