#include "GPColor.h"
#include "TestBinTree.h"

#include "FunctorFactory.h"

GPColor::GPColor() :
	randRed{ 0 },
	randGreen{ 0 },
	randBlue{ 0 },
	redTree{ std::make_shared<DivisionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	greenTree{ std::make_shared<AdditionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	blueTree{ std::make_shared<SubtractionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() }
{}

void GPColor::Initialize(const std::shared_ptr<FunctorBase> redFunctor, const std::shared_ptr<FunctorBase> greenFunctor, const std::shared_ptr<FunctorBase> blueFunctor)
{
	//(x / y) / r
	redTree.getRoot().getNodePtr()->setObj(redFunctor);
	redTree.insert(redFunctor);
	redTree.insert(redFunctor);

	redTree.insert(std::make_shared<FunctorBase>(&posX));
	redTree.insert(std::make_shared<FunctorBase>(&randRed));
	redTree.insert(std::make_shared<FunctorBase>(&randRed));
	redTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x / y) * r
	greenTree.getRoot().getNodePtr()->setObj(greenFunctor);
	greenTree.insert(greenFunctor);
	greenTree.insert(greenFunctor);

	greenTree.insert(std::make_shared<FunctorBase>(&posX));
	greenTree.insert(std::make_shared<FunctorBase>(&randGreen));
	greenTree.insert(std::make_shared<FunctorBase>(&randGreen));
	greenTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x * y) / r
	blueTree.getRoot().getNodePtr()->setObj(blueFunctor);
	blueTree.insert(blueFunctor);
	blueTree.insert(blueFunctor);

	blueTree.insert(std::make_shared<FunctorBase>(&posX));
	blueTree.insert(std::make_shared<FunctorBase>(&randBlue));
	blueTree.insert(std::make_shared<FunctorBase>(&randBlue));
	blueTree.insert(std::make_shared<FunctorBase>(&posY));
}

void GPColor::SetPosition(sf::Vector2f position)
{
	posX = position.x;
	posY = position.y;
}

sf::Color GPColor::GetColor() const
{
	uint8_t red = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(redTree, redTree.getRoot())));

	uint8_t green = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(greenTree, greenTree.getRoot())));

	uint8_t blue = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(blueTree, blueTree.getRoot())));

	return sf::Color{ red, green, blue };
}

void GPColor::Evolve(const GPColor& parentColor, const std::vector<FunctorBase::FunctorTypes>& availableFunctors)
{
	//slightly mutate one of 'r's
	switch (rand() % 3) {
	case 0:
		randRed = parentColor.randRed + ((100 - (rand() % 201)) / 10.0f);
		break;
	case 1:
		randGreen = parentColor.randGreen + ((100 - (rand() % 201)) / 10.0f);
		break;
	case 3:
		randBlue = parentColor.randBlue + ((100 - (rand() % 201)) / 10.0f);
		break;
	}

	//mutation chance for each of trees, mutate only one of them
	switch (rand() % 3) {
	case 0:
		if (rand() % 100 < 10)
			TestBinTree::getRandFunctorNode(redTree, redTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
		break;
	case 1:
		if (rand() % 100 < 10)
			TestBinTree::getRandFunctorNode(greenTree, greenTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
		break;
	case 2:
		if (rand() % 100 < 10)
			TestBinTree::getRandFunctorNode(blueTree, blueTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
		break;
	}
}