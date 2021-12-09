#include <iostream>
#include <cmath>
#include <set>
#include <algorithm>

#include "BoidFlock.h"

#include "MathAdditions.h"
#include "Definitions.h"

using mathAdditions::VectorMagnitude;
using mathAdditions::VectorSqrMagnitude;

BoidFlock::BoidFlock(std::map<FlockBehaviourTypes, FlockBehaviour*> flockRules) :
	boidsVerticesArr{ sf::PrimitiveType::Points, BOIDS_COUNT },
	flockBehaviours{ flockRules },
	ENABLE_TRAILS{ false }
{
	//initialize positions and vertex array
	for (int i = 0; i < BOIDS_COUNT; ++i) {
		boidsDataArr[i].position = sf::Vector2f((float)(rand() % dh::definitions::windowSizeX), (float)(rand() % dh::definitions::windowSizeY));
	}
	for (int i = 0; i < BOIDS_COUNT; ++i) {
		boidsVerticesArr[i] = (sf::Vertex{ boidsDataArr[i].position, sf::Color::Transparent });
	}
}

BoidFlock::~BoidFlock()
{
	//memory cleanup
	for (auto& behaviour : flockBehaviours) {
		delete behaviour.second;
	}
}

void BoidFlock::Update()
{
	try {
		for (int i = 0; i < BOIDS_COUNT; ++i) {

			std::vector<BoidAgentData*> boidsInView{ GetBoidsInView(boidsDataArr[i]) };

			//calculate all added behaviours
			std::map<FlockBehaviourTypes, sf::Vector2f> calculatedMovements;
			std::vector<sf::Vector2f> calculatedMovementsVecs;
			sf::Vector2f accelerationVec{ 0,0 };

			for (auto& behaviour : flockBehaviours) {
				calculatedMovements.insert({ behaviour.first,  (*behaviour.second)(boidsDataArr[i], boidsInView) });
				calculatedMovementsVecs.push_back(calculatedMovements[behaviour.first]);
				accelerationVec += calculatedMovements[behaviour.first];
			}

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

			sf::Vector2f maxVec{ 0,0 };
			for (int i = 0; i < calculatedMovementsVecs.size(); ++i) {
				maxVec = std::max(calculatedMovementsVecs[i], maxVec, compFunc);
			}

			constexpr float colorBias = 100;
			float alignStr = VectorSqrMagnitude(calculatedMovements[FlockBehaviourTypes::Alignment]) / VectorSqrMagnitude(maxVec);
			float cohesStr = VectorSqrMagnitude(calculatedMovements[FlockBehaviourTypes::Cohesion]) / VectorSqrMagnitude(maxVec);
			float separStr = VectorSqrMagnitude(calculatedMovements[FlockBehaviourTypes::Separation]) / VectorSqrMagnitude(maxVec);
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
