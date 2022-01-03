#include "GPColor.h"
#include "TestBinTree.h"

GPColor::GPColor() : r{ static_cast<float>(1 + rand() % 255) }
{
}

sf::Color GPColor::GetColor(sf::Vector2f position)
{
	posX = position.x;
	posY = position.y;

	BinTree<FunctorBase*> redTree(new DivisionFunctor(), BinTree<FunctorBase*>(), BinTree<FunctorBase*>());
	redTree.insert(new DivisionFunctor());
	redTree.insert(new FunctorBase(r));
	redTree.insert(new FunctorBase(posX));
	redTree.insert(new FunctorBase(posY));

	uint8_t red = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<FunctorBase*>(redTree, redTree.getRoot())) % 256);

	BinTree<FunctorBase*> greenTree(new MultiplicationFunctor(), BinTree<FunctorBase*>(), BinTree<FunctorBase*>());
	greenTree.insert(new DivisionFunctor());
	greenTree.insert(new FunctorBase(r));
	greenTree.insert(new FunctorBase(posX));
	greenTree.insert(new FunctorBase(posY));

	uint8_t green = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<FunctorBase*>(greenTree, greenTree.getRoot())) % 256);

	BinTree<FunctorBase*> blueTree(new DivisionFunctor(), BinTree<FunctorBase*>(), BinTree<FunctorBase*>());
	blueTree.insert(new MultiplicationFunctor());
	blueTree.insert(new FunctorBase(r));
	blueTree.insert(new FunctorBase(posX));
	blueTree.insert(new FunctorBase(posY));

	uint8_t blue = static_cast<uint8_t>(static_cast<int>(TestBinTree::calculate<FunctorBase*>(greenTree, greenTree.getRoot())) % 256);

	return sf::Color{ red, green, blue };
}