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
	BoidFlock(const BoidFlock& other);
	BoidFlock(size_t flockSize, std::map<FlockBehaviourTypes, std::shared_ptr<FlockBehaviour>> flockRules);
	BoidFlock& operator = (BoidFlock other);
	~BoidFlock() = default;
	void MoveBoids();

private:
	std::vector<BoidAgentData*> GetBoidsInView(const BoidAgentData& boid);
	sf::Vector2f CalculateBoidMovement(const BoidAgentData& boid);
public:
	//parameters
	static const float SQUARE_NEIGHBOUR_AVOIDANCE_RADIUS;
	static const float SQUARE_BOIDS_VIEW_RANGE;
	static const float MAX_SPEED;
	static const float MAX_ACCELERATION_CHANGE; //range between 0-1f

	bool ENABLE_TRAILS;

	//variables
	std::vector<BoidAgentData> boidsDataArr;

	std::map<FlockBehaviourTypes, std::shared_ptr<FlockBehaviour>> flockBehaviours;
	sf::VertexArray boidsVerticesArr;
	sf::RenderTexture renderTexture;
};


