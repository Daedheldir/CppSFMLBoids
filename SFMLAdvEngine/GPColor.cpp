#include "GPColor.h"
#include "TestBinTree.h"

GPColor::GPColor() :
	r{ static_cast<float>(1 + rand() % 255) },
	redTree{ std::make_shared<DivisionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	greenTree{ std::make_shared<MultiplicationFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	blueTree{ std::make_shared<DivisionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() }
{
	//(x + y) / r
	redTree.insert(std::make_shared<DivisionFunctor>());
	redTree.insert(std::make_shared<FunctorBase>(&r));
	redTree.insert(std::make_shared<FunctorBase>(&posX));
	redTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x / y) * r
	greenTree.insert(std::make_shared<DivisionFunctor>());
	greenTree.insert(std::make_shared<FunctorBase>(&r));
	greenTree.insert(std::make_shared<FunctorBase>(&posX));
	greenTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x * y) / r
	blueTree.insert(std::make_shared<MultiplicationFunctor>());
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

void GPColor::Evolve(const GPColor& parentColor)
{
	//mutation chance of 10% for each of trees
	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(redTree, redTree.getRoot())->setObj(GetRandFunctor());

	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(greenTree, greenTree.getRoot())->setObj(GetRandFunctor());

	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(blueTree, blueTree.getRoot())->setObj(GetRandFunctor());

	//slightly mutate 'r'
	r = parentColor.r + (5 - rand() % 10);
}

std::shared_ptr<FunctorBase> GPColor::GetRandFunctor()
{
	int choice = rand() % 3;

	switch (choice)
	{
	case 0:
	{
		return std::make_shared<AdditionFunctor>();
	}
	case 1:
	{
		return std::make_shared<MultiplicationFunctor>();
	}
	case 2:
	{
		return std::make_shared<DivisionFunctor>();
	}
	}
}