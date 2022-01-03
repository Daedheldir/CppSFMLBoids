#pragma once
#include <SFML/Graphics.hpp>
#include "BinTree.h"

class GPColor {
public:
	GPColor();
	GPColor(const std::vector<FunctorBase::FunctorTypes>& availableFunctors);
	sf::Color GetColor(sf::Vector2f position);
	void Evolve(const GPColor& parentColor, const std::vector<FunctorBase::FunctorTypes>& availableFunctors);
private:
	std::shared_ptr<FunctorBase> GetRandFunctor(const std::vector<FunctorBase::FunctorTypes>& availableFunctors);
public:
	float posX = 0;
	float posY = 0;
	float r = 0;
private:
	BinTree < std::shared_ptr<FunctorBase>> redTree;
	BinTree < std::shared_ptr<FunctorBase>> greenTree;
	BinTree < std::shared_ptr<FunctorBase>> blueTree;
};
