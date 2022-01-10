#pragma once
#include <SFML/Graphics.hpp>
#include "BinTree.h"

class GPColor {
public:
	GPColor();
	void Initialize(const std::vector<FunctorBase::FunctorTypes>& availableFunctors);
	void SetPosition(sf::Vector2f position);
	sf::Color GetColor() const;
	void Evolve(const GPColor& parentColor, const std::vector<FunctorBase::FunctorTypes>& availableFunctors);
private:
public:
	float posX = 0;
	float posY = 0;
	float randRed = 0;
	float randGreen = 0;
	float randBlue = 0;
private:
	BinTree < std::shared_ptr<FunctorBase>> redTree;
	BinTree < std::shared_ptr<FunctorBase>> greenTree;
	BinTree < std::shared_ptr<FunctorBase>> blueTree;
};
