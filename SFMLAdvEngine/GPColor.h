#pragma once
#include <SFML/Graphics.hpp>
#include "BinTree.h"
#include "AdditionFunctor.h"
#include "DivisionFunctor.h"
#include "MultiplicationFunctor.h"

class GPColor : protected sf::Color {
public:
	GPColor();
	sf::Color GetColor(sf::Vector2f position);
public:
	float posX = 0;
	float posY = 0;
	float r = 0;
	//BinTree < std::shared_ptr<FunctorBase>> redTree;
	//BinTree < std::shared_ptr<FunctorBase>> greenTree;
	//BinTree < std::shared_ptr<FunctorBase>> blueTree;
};
