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
const float BoidFlock::NEIGHBOUR_AVOIDANCE_RADIUS = sqrtf(SQUARE_NEIGHBOUR_AVOIDANCE_RADIUS);
const float BoidFlock::BOIDS_VIEW_RANGE = sqrtf(SQUARE_BOIDS_VIEW_RANGE);
const float BoidFlock::MAX_SPEED = 0.8f;
const float BoidFlock::MAX_ACCELERATION_CHANGE = 0.05f; //range between 0-1f

BoidFlock::BoidFlock(const BoidFlock& other) :
	boidsDataArr{ other.boidsDataArr },
	boidsVerticesArr{ other.boidsVerticesArr },
	flockBehaviours{ other.flockBehaviours },
	boidPosBounds{ other.boidPosBounds },
	boidsQuadtree{ {
		{boidPosBounds.x / 2.0f, boidPosBounds.y / 2.0f },
		{boidPosBounds.x + 2.0f, boidPosBounds.y + 2.0f}
		} }
{
}

BoidFlock::BoidFlock(size_t flockSize, std::map<FlockBehaviourTypes, std::shared_ptr<FlockBehaviour>> flockRules, sf::Vector2u boidPositionBound) :
	boidsVerticesArr{ sf::PrimitiveType::Points, flockSize },
	flockBehaviours{ flockRules },
	boidPosBounds{ boidPositionBound },
	boidsQuadtree{ {
		{boidPosBounds.x / 2.0f, boidPosBounds.y / 2.0f },
		{boidPosBounds.x + 2.0f, boidPosBounds.y + 2.0f}
		} }

{
	//initialize positions and vertex array
	boidsDataArr.resize(flockSize);
	int flockGroups = 100;
	int groupRadius = 100;

	for (int group = 0; group < flockGroups; ++group) {
		sf::Vector2f groupCenter{ (float)(rand() % boidPosBounds.x), (float)(rand() % boidPosBounds.y) };
		for (int i = 0; i < flockSize / flockGroups; ++i) {
			sf::Vector2f boidPos{
				(float)(groupCenter.x + (groupRadius / 2 - rand() % groupRadius)),
				(float)(groupCenter.y + (groupRadius / 2 - rand() % groupRadius))
			};

			if (boidPos.x < 0) boidPos.x = 0;
			else if (boidPos.x > boidPosBounds.x) boidPos.x = boidPosBounds.x;
			if (boidPos.y < 0) boidPos.y = 0;
			else if (boidPos.y > boidPosBounds.y) boidPos.y = boidPosBounds.y;

			boidsDataArr[i].position = boidPos;
		}
	}
	for (int i = 0; i < boidsDataArr.size(); ++i) {
		boidsQuadtree.insertCopy(&boidsDataArr[i]);
		boidsVerticesArr[i] = (sf::Vertex{ boidsDataArr[i].position, sf::Color::Transparent });
		boidsVerticesArr[i].color = sf::Color::Black;
	}
}
BoidFlock& BoidFlock::operator=(BoidFlock other) {
	std::swap(boidsDataArr, other.boidsDataArr);
	std::swap(boidsVerticesArr, other.boidsVerticesArr);
	std::swap(flockBehaviours, other.flockBehaviours);
	std::swap(boidPosBounds, other.boidPosBounds);

	return *this;
}

void BoidFlock::MoveBoids()
{
	UpdateTimer.fTreeUpdateTimer += 1;
	if (UpdateTimer.fTreeUpdateTimer >= UpdateTimer.fTreeUpdateInterval) {
		UpdateTimer.bRebuildTree = true;
		UpdateTimer.fTreeUpdateTimer -= UpdateTimer.fTreeUpdateInterval;
	}
	else {
		UpdateTimer.bRebuildTree = false;
	}

	try {
		for (int i = 0; i < boidsDataArr.size(); ++i) {
			//update boid pos
			boidsDataArr[i].acceleration = CalculateBoidMovement(boidsDataArr[i]);;
			boidsDataArr[i].position += boidsDataArr[i].acceleration;

			//preventing running off screen
			if (boidsDataArr[i].position.x < 0) boidsDataArr[i].position.x = (float)boidPosBounds.x;
			else if (boidsDataArr[i].position.x > boidPosBounds.x) boidsDataArr[i].position.x = 0.0f;
			if (boidsDataArr[i].position.y < 0) boidsDataArr[i].position.y = (float)boidPosBounds.y;
			else if (boidsDataArr[i].position.y > boidPosBounds.y) boidsDataArr[i].position.y = 0.0f;

			boidsVerticesArr[i].position = boidsDataArr[i].position;

			if (UpdateTimer.bRebuildTree) {
				BoidAgentData* p = &boidsDataArr[i];
				boidsDataArr[i].containingNode->checkIfNodeChanged(p);
			}
		}
	}
	catch (std::out_of_range e) {
		return;
	}
}

sf::Vector2f BoidFlock::CalculateBoidMovement(const BoidAgentData& boid) {
	std::vector<BoidAgentData*> boidsInView;
	GetBoidsInView<NeighbourFindingMethod::QuadtreeQueryRadius>(boid, boidsInView);

	//calculate all added behaviours
	sf::Vector2f accelerationVec{ 0,0 };

	for (auto& behaviour : flockBehaviours) {
		accelerationVec += (*behaviour.second)(boid, boidsInView);
	}

	//calculate acceleration change
	accelerationVec.x = std::lerp(boid.acceleration.x, accelerationVec.x, MAX_ACCELERATION_CHANGE);
	accelerationVec.y = std::lerp(boid.acceleration.y, accelerationVec.y, MAX_ACCELERATION_CHANGE);

	//clamp vector magnitude
	accelerationVec = mathAdditions::ClampVectorMagnitude(accelerationVec, MAX_SPEED);

	return accelerationVec;
}