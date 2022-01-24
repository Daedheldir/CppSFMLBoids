#include "GPColor.h"
#include "TestBinTree.h"

#include "FunctorFactory.h"
#include "GPParameters.h"

GPColor::GPColor() :
	randRed{ 0 },
	randGreen{ 0 },
	randBlue{ 0 },
	redTree{ std::make_shared<DivisionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	greenTree{ std::make_shared<AdditionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() },
	blueTree{ std::make_shared<SubtractionFunctor>(), BinTree<std::shared_ptr<FunctorBase>>(), BinTree<std::shared_ptr<FunctorBase>>() }
{}

void GPColor::Initialize(const std::shared_ptr<FunctorBase> redFunctor[], const std::shared_ptr<FunctorBase> greenFunctor[], const std::shared_ptr<FunctorBase> blueFunctor[])
{
	//(x / y) / r
	redTree.getRoot().getNodePtr()->setObj(redFunctor[0]);
	redTree.insert(redFunctor[1]);
	redTree.insert(redFunctor[2]);

	redTree.insert(std::make_shared<FunctorBase>(&posX));
	redTree.insert(std::make_shared<FunctorBase>(&randRed));
	redTree.insert(std::make_shared<FunctorBase>(&randRed));
	redTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x / y) * r
	greenTree.getRoot().getNodePtr()->setObj(greenFunctor[0]);
	greenTree.insert(greenFunctor[1]);
	greenTree.insert(greenFunctor[2]);

	greenTree.insert(std::make_shared<FunctorBase>(&posX));
	greenTree.insert(std::make_shared<FunctorBase>(&randGreen));
	greenTree.insert(std::make_shared<FunctorBase>(&randGreen));
	greenTree.insert(std::make_shared<FunctorBase>(&posY));

	//(x * y) / r
	blueTree.getRoot().getNodePtr()->setObj(blueFunctor[0]);
	blueTree.insert(blueFunctor[1]);
	blueTree.insert(blueFunctor[2]);

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
	auto GetRandNumberInRange = [](const float& min, const float& max, const unsigned int& precision)->float {
		float diff = max - min;
		unsigned int precisionMultiplier = std::powl(10, precision);
		if (precision > 0)
			return min + (rand() % static_cast<unsigned int>(max * precisionMultiplier)) / static_cast<float>(precisionMultiplier);
		else
			return min + (rand() % static_cast<unsigned int>(max));
	};

	if (rand() % 100 < GPParameters::MUTATION_RATE_RAND * 100) {
		switch (rand() % 3) {
		case 0:
			randRed = parentColor.randRed + GetRandNumberInRange(GPParameters::MUTATION_RANGE_MIN, GPParameters::MUTATION_RANGE_MAX, 2);
			break;
		case 1:
			randGreen = parentColor.randGreen + GetRandNumberInRange(GPParameters::MUTATION_RANGE_MIN, GPParameters::MUTATION_RANGE_MAX, 2);
			break;
		case 3:
			randBlue = parentColor.randBlue + GetRandNumberInRange(GPParameters::MUTATION_RANGE_MIN, GPParameters::MUTATION_RANGE_MAX, 2);
			break;
		}
	}

	redTree.getRoot().getNodePtr()->setObj(std::shared_ptr<FunctorBase>{ parentColor.redTree.getRootObj() });
	redTree.getLeftPointer()->setObj(std::shared_ptr<FunctorBase>{ parentColor.redTree.getLeftObj() });
	redTree.getRightPointer()->setObj(std::shared_ptr<FunctorBase>{ parentColor.redTree.getRightObj() });

	greenTree.getRoot().getNodePtr()->setObj(std::shared_ptr<FunctorBase>{ parentColor.greenTree.getRootObj() });
	greenTree.getLeftPointer()->setObj(std::shared_ptr<FunctorBase>{ parentColor.greenTree.getLeftObj() });
	greenTree.getRightPointer()->setObj(std::shared_ptr<FunctorBase>{ parentColor.greenTree.getRightObj() });

	blueTree.getRoot().getNodePtr()->setObj(std::shared_ptr<FunctorBase>{ parentColor.blueTree.getRootObj() });
	blueTree.getLeftPointer()->setObj(std::shared_ptr<FunctorBase>{ parentColor.blueTree.getLeftObj() });
	blueTree.getRightPointer()->setObj(std::shared_ptr<FunctorBase>{ parentColor.blueTree.getRightObj() });

	//mutation chance for each of trees, mutate only one of them
	if (rand() % 100 < GPParameters::MUTATION_RATE_TREE * 100) {
		switch (std::rand() % 3) {
		case 0:
			for (int k = 0; k < GPParameters::MUTATION_NODES_COUNT; ++k)
				TestBinTree::getRandFunctorNode(redTree, redTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
			break;
		case 1:
			for (int k = 0; k < GPParameters::MUTATION_NODES_COUNT; ++k)
				TestBinTree::getRandFunctorNode(greenTree, greenTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
			break;
		case 2:
			for (int k = 0; k < GPParameters::MUTATION_NODES_COUNT; ++k)
				TestBinTree::getRandFunctorNode(blueTree, blueTree.getRoot())->setObj(FunctorFactory::CreateFunctor(availableFunctors[rand() % availableFunctors.size()]));
			break;
		}
	}
}