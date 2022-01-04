#include "GPColor.h"
#include "TestBinTree.h"

#include "FunctorFactory.h"

GPColor::GPColor() :
	GPColor{ {{
			FunctorBase::FunctorTypes::Addition,
			FunctorBase::FunctorTypes::Multiplication,
			FunctorBase::FunctorTypes::Division,
			FunctorBase::FunctorTypes::BitwiseAND,
			FunctorBase::FunctorTypes::BitwiseOR,
			FunctorBase::FunctorTypes::Subtraction,
			FunctorBase::FunctorTypes::LeftShift,
			FunctorBase::FunctorTypes::Sine
}} }
{
}

GPColor::GPColor(const std::vector<FunctorBase::FunctorTypes>& availableFunctors) :
	randRed{ static_cast<float>((-100 + rand() % 200) / 100) },
	randGreen{ static_cast<float>((-100 + rand() % 200) / 100) },
	randBlue{ static_cast<float>((-100 + rand() % 200) / 100) },
	redTree{ std::make_shared<AdditionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	greenTree{ std::make_shared<AdditionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	blueTree{ std::make_shared<AdditionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() }
{
	//(x / y) / r
	redTree.getRoot().getNodePtr()->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
	redTree.insert(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
	redTree.insert(std::make_shared<FunctorBase>(&randRed));
	redTree.insert(std::make_shared<FunctorBase>(&posX));
	redTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x / y) * r
	greenTree.getRoot().getNodePtr()->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
	greenTree.insert(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
	greenTree.insert(std::make_shared<FunctorBase>(&randGreen));
	greenTree.insert(std::make_shared<FunctorBase>(&posX));
	greenTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x * y) / r
	blueTree.getRoot().getNodePtr()->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
	blueTree.insert(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
	blueTree.insert(std::make_shared<FunctorBase>(&randBlue));
	blueTree.insert(std::make_shared<FunctorBase>(&posX));
	blueTree.insert(std::make_shared<FunctorBase>(&posY));
}

void GPColor::SetPosition(sf::Vector2f position)
{
	posX = position.x;
	posY = position.y;
}

sf::Color GPColor::GetColor() const
{
	uint8_t red = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(redTree, redTree.getRoot())) % 256);

	uint8_t green = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(greenTree, greenTree.getRoot())) % 256);

	uint8_t blue = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<std::shared_ptr<FunctorBase>>(blueTree, blueTree.getRoot())) % 256);

	return sf::Color{ red, green, blue };
}

void GPColor::Evolve(const GPColor& parentColor, const std::vector<FunctorBase::FunctorTypes>& availableFunctors)
{
	//mutation chance for each of trees
	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(redTree, redTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));

	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(greenTree, greenTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));

	if (rand() % 100 < 10)
		TestBinTree::getRandFunctorNode(blueTree, blueTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));

	//slightly mutate 'r'
	randRed = parentColor.randRed + ((1000 - rand() % 2000) / 200.0f);
	randGreen = parentColor.randGreen + ((1000 - rand() % 2000) / 200.0f);
	randBlue = parentColor.randBlue + ((1000 - rand() % 2000) / 200.0f);
}