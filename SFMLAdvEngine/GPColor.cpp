#include "GPColor.h"
#include "TestBinTree.h"

#include "AdditionFunctor.h"
#include "DivisionFunctor.h"
#include "MultiplicationFunctor.h"
#include "SubtractionFunctor.h"
#include "BitwiseANDFunctor.h"
#include "BitwiseORFunctor.h"
#include "SineFunctor.h"
#include "LeftShiftFunctor.h"

GPColor::GPColor() :
	GPColor{ {{
			FunctorBase::FunctorTypes::Addition,
			FunctorBase::FunctorTypes::Multiplication,
			FunctorBase::FunctorTypes::Division
}} }
{
}

GPColor::GPColor(const std::vector<FunctorBase::FunctorTypes>& availableFunctors) :
	r{ static_cast<float>(1 + rand() % 255) },
	redTree{ std::make_shared<AdditionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	greenTree{ std::make_shared<AdditionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	blueTree{ std::make_shared<AdditionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() }
{
	//(x / y) / r
	redTree.getRoot().getNodePtr()->setObj(GetRandFunctor(availableFunctors));
	redTree.insert(GetRandFunctor(availableFunctors));
	redTree.insert(std::make_shared<FunctorBase>(&r));
	redTree.insert(std::make_shared<FunctorBase>(&posX));
	redTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x / y) * r
	greenTree.getRoot().getNodePtr()->setObj(GetRandFunctor(availableFunctors));
	greenTree.insert(GetRandFunctor(availableFunctors));
	greenTree.insert(std::make_shared<FunctorBase>(&r));
	greenTree.insert(std::make_shared<FunctorBase>(&posX));
	greenTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x * y) / r
	blueTree.getRoot().getNodePtr()->setObj(GetRandFunctor(availableFunctors));
	blueTree.insert(GetRandFunctor(availableFunctors));
	blueTree.insert(std::make_shared<FunctorBase>(&r));
	blueTree.insert(std::make_shared<FunctorBase>(&posX));
	blueTree.insert(std::make_shared<FunctorBase>(&posY));
}

sf::Color GPColor::GetColor(sf::Vector2f position)
{
	posX = position.x;
	posY = position.y;

	uint8_t red = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(redTree, redTree.getRoot())) % 256);

	uint8_t green = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(greenTree, greenTree.getRoot())) % 256);

	uint8_t blue = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(greenTree, greenTree.getRoot())) % 256);

	return sf::Color{ red, green, blue };
}

void GPColor::Evolve(const GPColor& parentColor, const std::vector<FunctorBase::FunctorTypes>& availableFunctors)
{
	//mutation chance for each of trees
	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(redTree, redTree.getRoot())->setObj(GetRandFunctor(availableFunctors));

	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(greenTree, greenTree.getRoot())->setObj(GetRandFunctor(availableFunctors));

	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(blueTree, blueTree.getRoot())->setObj(GetRandFunctor(availableFunctors));

	//slightly mutate 'r'
	r = parentColor.r + ((1000 - rand() % 2000) / 200.0f);
}

std::shared_ptr<FunctorBase> GPColor::GetRandFunctor(const std::vector<FunctorBase::FunctorTypes>& availableFunctors)
{
	int choice = rand() % availableFunctors.size();

	switch (availableFunctors[choice])
	{
	case FunctorBase::FunctorTypes::Addition:
	{
		return std::make_shared<AdditionFunctor>();
	}
	case FunctorBase::FunctorTypes::Multiplication:
	{
		return std::make_shared<MultiplicationFunctor>();
	}
	case FunctorBase::FunctorTypes::Division:
	{
		return std::make_shared<DivisionFunctor>();
	}
	case FunctorBase::FunctorTypes::Subtraction:
	{
		return std::make_shared<SubtractionFunctor>();
	}
	case FunctorBase::FunctorTypes::BitwiseAND:
	{
		return std::make_shared<BitwiseANDFunctor>();
	}
	case FunctorBase::FunctorTypes::BitwiseOR:
	{
		return std::make_shared<BitwiseORFunctor>();
	}
	case FunctorBase::FunctorTypes::Sine:
	{
		return std::make_shared<SineFunctor>();
	}
	case FunctorBase::FunctorTypes::LeftShift:
	{
		return std::make_shared<LeftShiftFunctor>();
	}
	}
}