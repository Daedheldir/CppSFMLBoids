#include <iostream>
#include <cmath>
#include <set>

#include "BoidFlock.h"

#include "MathAdditions.h"
#include "Definitions.h"

using mathAdditions::VectorMagnitude;
using mathAdditions::VectorSqrMagnitude;

BoidFlock::BoidFlock() : boidsVerticesArr{ sf::PrimitiveType::Points, BOIDS_COUNT }
{
	//initialize positions and vertex array
	for (int i = 0; i < BOIDS_COUNT; ++i) {
		boidsDataArr[i].position = sf::Vector2f((float)(rand() % dh::definitions::windowSizeX), (float)(rand() % dh::definitions::windowSizeY));
	}
	for (int i = 0; i < BOIDS_COUNT; ++i) {
		boidsVerticesArr[i] = (sf::Vertex{ boidsDataArr[i].position, sf::Color::Transparent });
	}
}

void BoidFlock::Update()
{
	try {
		for (int i = 0; i < BOIDS_COUNT; ++i) {

			std::vector<BoidAgentData*> boidsInView{ GetBoidsInView(boidsDataArr[i]) };

			//calculate alignment for all boids
			sf::Vector2f alignmentVec = CalculateAlignment(boidsDataArr[i], boidsInView) * ALIGNMENT_WEIGHT;

			//calculate cohesion for all agents
			sf::Vector2f cohesionVec = CalculateCohesion(boidsDataArr[i], boidsInView) * COHESION_WEIGHT;

			//calculate separation for all agents
			sf::Vector2f separationVec = CalculateSeparation(boidsDataArr[i], boidsInView) * SEPARATION_WEIGHT;

			//add some randomness to their movement
			sf::Vector2f randVec = CalculateRandomMovement() * RAND_MOVE_WEIGHT;

			sf::Vector2f accelerationVec = alignmentVec + cohesionVec + separationVec + randVec;

			//calculate acceleration change
			accelerationVec.x = std::lerp(boidsDataArr[i].acceleration.x, accelerationVec.x, MAX_ACCELERATION_CHANGE);
			accelerationVec.y = std::lerp(boidsDataArr[i].acceleration.y, accelerationVec.y, MAX_ACCELERATION_CHANGE);


			//clamp vector magnitude
			accelerationVec = mathAdditions::ClampVectorMagnitude(accelerationVec, MAX_SPEED);

			//preventing running off screen
			sf::Vector2u boidPosBounds{ dh::definitions::windowSizeX, dh::definitions::windowSizeY };
			sf::Vector2f boidPos{ boidsDataArr[i].position + accelerationVec };

			if (boidPos.x < 0) boidPos.x = (float)boidPosBounds.x;
			else if (boidPos.x > boidPosBounds.x) boidPos.x = 0.0f;
			if (boidPos.y < 0) boidPos.y = (float)boidPosBounds.y;
			else if (boidPos.y > boidPosBounds.y) boidPos.y = 0.0f;

			//update boid pos
			boidsDataArr[i].acceleration = accelerationVec;
			boidsDataArr[i].position = boidPos;
			boidsVerticesArr[i].position = boidsDataArr[i].position;

			//do some coloring
			auto compFunc = [](const sf::Vector2f& a, const sf::Vector2f& b) -> bool
			{
				float mag_a = VectorSqrMagnitude(a);
				float mag_b = VectorSqrMagnitude(b);
				return  mag_a < mag_b;
			};
			sf::Vector2f maxVec = { std::max
			(
				std::initializer_list{ alignmentVec, cohesionVec, separationVec },
				compFunc)
			};
			constexpr float colorBias = 100;
			float alignStr = VectorSqrMagnitude(alignmentVec) / VectorSqrMagnitude(maxVec);
			float cohesStr = VectorSqrMagnitude(cohesionVec) / VectorSqrMagnitude(maxVec);
			float separStr = VectorSqrMagnitude(separationVec) / VectorSqrMagnitude(maxVec);
			float speedMag = mathAdditions::MapValue(VectorSqrMagnitude(accelerationVec), { 0.0f, MAX_SPEED }, { 0.0f, 1.0f });

			boidsVerticesArr[i].color = {
				static_cast<uint8_t>((255 - colorBias) * separStr + colorBias * speedMag),
				static_cast<uint8_t>((255 - colorBias) * cohesStr + colorBias * speedMag),
				static_cast<uint8_t>((255 - colorBias) * alignStr + colorBias * speedMag)
			};
		}
	}
	catch (std::out_of_range e) {
		return;
	}
}

std::vector<BoidAgentData*> BoidFlock::GetBoidsInView(const BoidAgentData& boid)
{
	std::vector<BoidAgentData*> boidsInView;
	for (auto& neighbour : boidsDataArr) {
		if (&neighbour != &boid && mathAdditions::VectorSqrMagnitude(boid.position - neighbour.position) < BoidFlock::SQUARE_BOIDS_VIEW_RANGE) {
			boidsInView.push_back(&neighbour);
		}
	}
	return boidsInView;
}

sf::Vector2f BoidFlock::CalculateAlignment(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView)
{
	if (boidsInView.size() <= 0) return currentBoid.acceleration;

	sf::Vector2f alignmentMove(0, 0);

	for (auto& heading : boidsInView) {
		alignmentMove += heading->acceleration;
	}

	alignmentMove *= 1.0f / boidsInView.size();

	return alignmentMove;
}

sf::Vector2f BoidFlock::CalculateCohesion(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView)
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

	return cohesionMove;
}

sf::Vector2f BoidFlock::CalculateSeparation(const BoidAgentData& currentBoid, const std::vector<BoidAgentData*>& boidsInView)
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

	return avoidanceMove;
}

sf::Vector2f BoidFlock::CalculateRandomMovement()
{
	sf::Vector2 randomMove{ 1 - (rand() % 200) / 100.0f,1 - (rand() % 200) / 100.0f };
	return randomMove;
}
