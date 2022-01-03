#pragma once
#include <SFML/Graphics.hpp>
#include "BinTree.h"

class GPColor : protected sf::Color {
public:
	GPColor();
	sf::Color GetColor(sf::Vector2f position);
	void Evolve(const GPColor& parentColor);
private:
	std::shared_ptr<FunctorBase> GetRandFunctor();
public:
	float posX = 0;
	float posY = 0;
	float r = 0;
private:
	BinTree < std::shared_ptr<FunctorBase>> redTree;
	BinTree < std::shared_ptr<FunctorBase>> greenTree;
	BinTree < std::shared_ptr<FunctorBase>> blueTree;
};
