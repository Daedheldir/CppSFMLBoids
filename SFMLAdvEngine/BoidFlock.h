#pragma once
#include <array>
#include <map>
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>
#include "BoidAgentData.h"
#include "FlockBehaviour.h"

enum class FlockBehaviourTypes {
	Alignment,
	Cohesion,
	Separation,
	RandomMovement
};

class BoidFlock
{
public:
	BoidFlock(size_t flockSize, std::map<FlockBehaviourTypes, FlockBehaviour*> flockRules);
	~BoidFlock();
	void Update();

private:
	std::vector<BoidAgentData*> GetBoidsInView(const BoidAgentData& boid);

	sf::Vector2f CalculateAlignment(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView);
	sf::Vector2f CalculateCohesion(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView);
	sf::Vector2f CalculateSeparation(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView);
	sf::Vector2f CalculateRandomMovement();
public:
	//parameters
	static constexpr float SQUARE_NEIGHBOUR_AVOIDANCE_RADIUS = 15.0f;
	static constexpr float SQUARE_BOIDS_VIEW_RANGE = 400.0f;
	static constexpr float MAX_SPEED = 0.5f;
	static constexpr float MAX_ACCELERATION_CHANGE = 0.05f; //range between 0-1f

	bool ENABLE_TRAILS;

	//variables
	std::vector<BoidAgentData> boidsDataArr;

	std::map<FlockBehaviourTypes, FlockBehaviour*> flockBehaviours;
	sf::VertexArray boidsVerticesArr;
};


