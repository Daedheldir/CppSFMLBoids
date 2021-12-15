#include <iostream>
#include <cmath>
#include <set>
#include <algorithm>

#include "BoidFlock.h"

#include "MathAdditions.h"
#include "Definitions.h"

using mathAdditions::VectorMagnitude;
using mathAdditions::VectorSqrMagnitude;

const float BoidFlock::SQUARE_NEIGHBOUR_AVOIDANCE_RADIUS = 15.0f;
const float BoidFlock::SQUARE_BOIDS_VIEW_RANGE = 400.0f;
const float BoidFlock::MAX_SPEED = 1.0f;
const float BoidFlock::MAX_ACCELERATION_CHANGE = 0.05f; //range between 0-1f

BoidFlock::BoidFlock(const BoidFlock& other) :
	boidsDataArr{ other.boidsDataArr },
	boidsVerticesArr{ other.boidsVerticesArr },
	flockBehaviours{ other.flockBehaviours },
	ENABLE_TRAILS{ other.ENABLE_TRAILS }
{
	renderTexture.create(other.renderTexture.getSize().x, other.renderTexture.getSize().y);
}

BoidFlock::BoidFlock(size_t flockSize, std::map<FlockBehaviourTypes, std::shared_ptr<FlockBehaviour>> flockRules) :
	boidsVerticesArr{ sf::PrimitiveType::Points, flockSize },
	flockBehaviours{ flockRules },
	ENABLE_TRAILS{ false }
{
	//initialize positions and vertex array
	boidsDataArr.resize(flockSize);

	for (int i = 0; i < flockSize; ++i) {
		boidsDataArr[i].position = sf::Vector2f((float)(rand() % dh::definitions::windowSizeX), (float)(rand() % dh::definitions::windowSizeY));
	}
	for (int i = 0; i < flockSize; ++i) {
		boidsVerticesArr[i] = (sf::Vertex{ boidsDataArr[i].position, sf::Color::Transparent });
		boidsVerticesArr[i].color = boidsDataArr[i].color;
	}

	renderTexture.create(dh::definitions::windowSizeX + 1, dh::definitions::windowSizeY + 1);
}
BoidFlock& BoidFlock::operator=(BoidFlock other) {
	std::swap(boidsDataArr, other.boidsDataArr);
	std::swap(boidsVerticesArr, other.boidsVerticesArr);
	std::swap(flockBehaviours, other.flockBehaviours);
	std::swap(ENABLE_TRAILS, other.ENABLE_TRAILS);
	renderTexture.create(other.renderTexture.getSize().x, other.renderTexture.getSize().y);

	return *this;
}


void BoidFlock::MoveBoids()
{
	try {
		for (int i = 0; i < boidsDataArr.size(); ++i) {
			//update boid pos
			boidsDataArr[i].acceleration = CalculateBoidMovement(boidsDataArr[i]);;
			boidsDataArr[i].position += boidsDataArr[i].acceleration;

			//preventing running off screen
			sf::Vector2u boidPosBounds{ dh::definitions::windowSizeX, dh::definitions::windowSizeY };

			if (boidsDataArr[i].position.x < 0) boidsDataArr[i].position.x = (float)boidPosBounds.x;
			else if (boidsDataArr[i].position.x > boidPosBounds.x) boidsDataArr[i].position.x = 0.0f;
			if (boidsDataArr[i].position.y < 0) boidsDataArr[i].position.y = (float)boidPosBounds.y;
			else if (boidsDataArr[i].position.y > boidPosBounds.y) boidsDataArr[i].position.y = 0.0f;

			boidsVerticesArr[i].position = boidsDataArr[i].position;
		}
	}
	catch (std::out_of_range e) {
		return;
	}
}


sf::Vector2f BoidFlock::CalculateBoidMovement(const BoidAgentData& boid) {
	std::vector<BoidAgentData*> boidsInView{ GetBoidsInView(boid) };

	//calculate all added behaviours
	std::map<FlockBehaviourTypes, sf::Vector2f> calculatedMovements;
	std::vector<sf::Vector2f> calculatedMovementsVecs;
	sf::Vector2f accelerationVec{ 0,0 };

	for (auto& behaviour : flockBehaviours) {
		calculatedMovements.insert({ behaviour.first,  (*behaviour.second)(boid, boidsInView) });
		calculatedMovementsVecs.push_back(calculatedMovements[behaviour.first]);
		accelerationVec += calculatedMovements[behaviour.first];
	}

	//calculate acceleration change
	accelerationVec.x = std::lerp(boid.acceleration.x, accelerationVec.x, MAX_ACCELERATION_CHANGE);
	accelerationVec.y = std::lerp(boid.acceleration.y, accelerationVec.y, MAX_ACCELERATION_CHANGE);

	//clamp vector magnitude
	accelerationVec = mathAdditions::ClampVectorMagnitude(accelerationVec, MAX_SPEED);

	return accelerationVec;
}

std::vector<BoidAgentData*> BoidFlock::GetBoidsInView(const BoidAgentData& boid)
{
	std::vector<BoidAgentData*> boidsInView;
	for (auto& neighbour : boidsDataArr) {
		if (&neighbour != &boid) {
			sf::Vector2f relativeVec = boid.position - neighbour.position;

			if (relativeVec.x < BoidFlock::SQUARE_BOIDS_VIEW_RANGE
				&& relativeVec.y < BoidFlock::SQUARE_BOIDS_VIEW_RANGE
				&& mathAdditions::VectorSqrMagnitude(relativeVec) < BoidFlock::SQUARE_BOIDS_VIEW_RANGE)
			{
				boidsInView.push_back(&neighbour);
			}
		}
	}
	return boidsInView;
}
