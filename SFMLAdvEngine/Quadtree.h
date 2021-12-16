#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>

#include "SFML\Graphics.hpp"

#include <mutex>

const size_t QT_BASE_BUCKET_SIZE = 2;
const size_t QT_BASE_DIVIDER = 2;
const unsigned int MAX_TREE_DEPTH = ~0;

struct AABB {
	AABB(sf::Vector2f center, sf::Vector2f halfDimensions) :
		center(center),
		halfDimensions(halfDimensions),
		rect(this->center.x - this->halfDimensions.x, this->center.y - this->halfDimensions.y, 2 * this->halfDimensions.x, 2 * this->halfDimensions.y)
	{};
	AABB(const AABB& boundaries) :
		center(boundaries.center),
		halfDimensions(boundaries.halfDimensions),
		rect(boundaries.rect)
	{};

	sf::Vector2f	center;
	sf::Vector2f	halfDimensions;
	sf::FloatRect	rect;

	bool ContainsPoint(const sf::Vector2f& point) const
	{
		if (rect.contains(point)) {
			return true;
		}
		return false;
	};

	bool Intersects(const AABB& other) const
	{
		if (rect.intersects(other.rect)) {
			return true;
		}
		return false;
	};
};
template<typename T>
class Quadtree
{
public:
	Quadtree(const AABB& boundaries, sf::Color color = sf::Color::Red)
		:Quadtree{ nullptr, boundaries, 0, color }
	{};

	Quadtree(Quadtree* parentNode, const AABB& boundaries, unsigned int depth, sf::Color color = sf::Color::Red)
		: parentNode{ parentNode },
		NodeNW{ nullptr },
		NodeNE{ nullptr },
		NodeSW{ nullptr },
		NodeSE{ nullptr },
		boundaries(boundaries)
	{
		sf::Vector2f boundariesVertex;

		//NW
		boundariesVertex.x = this->boundaries.center.x - (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y - (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		//NE
		boundariesVertex.x = this->boundaries.center.x + (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y - (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		//SE
		boundariesVertex.x = this->boundaries.center.x + (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y + (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		//SW
		boundariesVertex.x = this->boundaries.center.x - (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y + (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		//NW
		boundariesVertex.x = this->boundaries.center.x - (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y - (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		this->vertex.setPrimitiveType(sf::PrimitiveType::LineStrip);

		uDepth = depth;
		bucketSize = static_cast<size_t>(pow(QT_BASE_BUCKET_SIZE, uDepth / QT_BASE_DIVIDER));
		dataContainer.reserve(bucketSize);
	};
	Quadtree(Quadtree* parentNode, const sf::Vector2f& center, const sf::Vector2f& halfDimension, unsigned int depth, sf::Color color = sf::Color::Red)
		: Quadtree{ parentNode, {center, halfDimension}, depth, color }
	{
	};

	Quadtree(nullptr_t parentNode, const AABB& boundaries, unsigned int depth, sf::Color color = sf::Color::Red)
		: parentNode{ nullptr },
		NodeNW{ nullptr },
		NodeNE{ nullptr },
		NodeSW{ nullptr },
		NodeSE{ nullptr },
		boundaries(boundaries)
	{
		sf::Vector2f boundariesVertex;

		//NW
		boundariesVertex.x = this->boundaries.center.x - (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y - (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		//NE
		boundariesVertex.x = this->boundaries.center.x + (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y - (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		//SE
		boundariesVertex.x = this->boundaries.center.x + (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y + (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		//SW
		boundariesVertex.x = this->boundaries.center.x - (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y + (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		//NW
		boundariesVertex.x = this->boundaries.center.x - (this->boundaries.halfDimensions.x);
		boundariesVertex.y = this->boundaries.center.y - (this->boundaries.halfDimensions.y);

		this->vertex.append(sf::Vertex(boundariesVertex, color));

		this->vertex.setPrimitiveType(sf::PrimitiveType::LineStrip);

		uDepth = depth;
		bucketSize = static_cast<size_t>(pow(QT_BASE_BUCKET_SIZE, uDepth / QT_BASE_DIVIDER));
		dataContainer.reserve(bucketSize);
	};
	Quadtree(nullptr_t parentNode, const sf::Vector2f& center, const sf::Vector2f& halfDimension, unsigned int depth, sf::Color color = sf::Color::Red)
		: Quadtree{ parentNode, {center, halfDimension}, depth, color }
	{
	};
	~Quadtree() {};
	bool insertCopy(T data)
	{
		return insert(data);
	}
	bool insert(T& data)
	{
		//if the node does not containt the point return false
		if (!boundaries.ContainsPoint(data->position))
			return false;

		//if there's space in this node, add point here
		if (dataContainer.size() < bucketSize || uDepth >= MAX_TREE_DEPTH)
		{
			std::scoped_lock<std::recursive_mutex> dataLock(dataAccess);
			dataContainer.emplace_back(data);
			data->containingNode = this;
			return true;
		}

		//else if there's no space in this node, add the point to whichever node will accept it
		if (NodeNE == nullptr)
			subdivide();

		if (NodeNE->insert(data))
			return true;
		if (NodeNW->insert(data))
			return true;
		if (NodeSE->insert(data))
			return true;
		if (NodeSW->insert(data))
			return true;

		//if for some unknown reason node cannot be added throw exception
		throw std::exception("ERROR! Quadtree can't insert node!\n");
	};
	bool insertIntoThisNode(T& data)
	{
		//if there's space in this node, add point here
		if (dataContainer.size() < bucketSize || uDepth >= MAX_TREE_DEPTH)
		{
			std::scoped_lock<std::recursive_mutex> dataLock(dataAccess);

			dataContainer.emplace_back(data);
			data->containingNode = this;
			return true;
		}
		return false;
	};

	void subdivide()
	{
		sf::Vector2f subnodeCenter;
		sf::Vector2f subnodeHalfDimension;

		//NW
		subnodeCenter.x = this->boundaries.center.x - (this->boundaries.halfDimensions.x / 2.0f);
		subnodeCenter.y = this->boundaries.center.y - (this->boundaries.halfDimensions.y / 2.0f);
		subnodeHalfDimension = boundaries.halfDimensions / 2.0f;

		this->NodeNW = std::make_shared<Quadtree>(this, AABB(subnodeCenter, subnodeHalfDimension), uDepth + 1, vertex[0].color);
		if (NodeNW == nullptr)
		{
			throw std::exception("ERROR! Quadtree can't create node!\n");
		}

		//NE
		subnodeCenter.x = this->boundaries.center.x + (this->boundaries.halfDimensions.x / 2.0f);
		subnodeCenter.y = this->boundaries.center.y - (this->boundaries.halfDimensions.y / 2.0f);
		subnodeHalfDimension = boundaries.halfDimensions / 2.0f;

		this->NodeNE = std::make_shared<Quadtree>(this, AABB(subnodeCenter, subnodeHalfDimension), uDepth + 1, vertex[0].color);
		if (NodeNE == nullptr)
		{
			throw std::exception("ERROR! Quadtree can't create node!\n");
		}

		//SW
		subnodeCenter.x = this->boundaries.center.x - (this->boundaries.halfDimensions.x / 2.0f);
		subnodeCenter.y = this->boundaries.center.y + (this->boundaries.halfDimensions.y / 2.0f);
		subnodeHalfDimension = boundaries.halfDimensions / 2.0f;

		this->NodeSW = std::make_shared<Quadtree>(this, AABB(subnodeCenter, subnodeHalfDimension), uDepth + 1, vertex[0].color);
		if (NodeSW == nullptr)
		{
			throw std::exception("ERROR! Quadtree can't create node!\n");
		}

		//SE
		subnodeCenter.x = this->boundaries.center.x + (this->boundaries.halfDimensions.x / 2.0f);
		subnodeCenter.y = this->boundaries.center.y + (this->boundaries.halfDimensions.y / 2.0f);
		subnodeHalfDimension = boundaries.halfDimensions / 2.0f;

		this->NodeSE = std::make_shared<Quadtree>(this, AABB(subnodeCenter, subnodeHalfDimension), uDepth + 1, vertex[0].color);
		if (NodeSE == nullptr)
		{
			throw std::exception("ERROR! Quadtree can't create node!\n");
		}
	};

	unsigned int getPointCount() const
	{
		unsigned int pointCount;

		pointCount = static_cast<unsigned int>(dataContainer.size());

		if (NodeNE != nullptr)
		{
			pointCount += NodeNE->getPointCount();

			pointCount += NodeNW->getPointCount();

			pointCount += NodeSE->getPointCount();

			pointCount += NodeSW->getPointCount();
		}

		return pointCount;
	};

	void queryRange(const AABB& range, std::vector<T>& pointsInRange) const
	{
		if (!boundaries.Intersects(range))
			return;

		for (auto& iter : dataContainer)
		{
			if (range.ContainsPoint(iter->position))
				pointsInRange.emplace_back(iter);
		}

		if (NodeNW == nullptr)
			return;

		NodeNE->queryRange(range, pointsInRange);
		NodeNW->queryRange(range, pointsInRange);
		NodeSE->queryRange(range, pointsInRange);
		NodeSW->queryRange(range, pointsInRange);
	};
	void queryRange(const sf::Vector2f& center, const sf::Vector2f& halfDimension, std::vector<T>& pointsInRange) const
	{
		const AABB range{ center, halfDimension };
		queryRange(range, pointsInRange);
	};

	Quadtree* findContainingNode(T& searchedData)
	{
		Quadtree* node = nullptr;

		if (!boundaries.ContainsPoint(searchedData->position))
			return nullptr;

		std::unique_lock<std::recursive_mutex> dataLock(dataAccess);

		for (auto iter = this->dataContainer.begin(); iter != this->dataContainer.end(); ++iter)
		{
			if (*(*iter) == *searchedData)
				return this;
		}
		dataLock.unlock();

		if (NodeNE != nullptr)
		{
			node = NodeNE->findContainingNode(searchedData);

			if (node == nullptr)
				node = NodeNW->findContainingNode(searchedData);

			if (node == nullptr)
				node = NodeSE->findContainingNode(searchedData);

			if (node == nullptr)
				node = NodeSW->findContainingNode(searchedData);
		}

		return node;
	};

	bool remove(T& searchedData)
	{
		if (!boundaries.ContainsPoint(searchedData->position))
			return false;

		for (auto iter = dataContainer.begin(); iter != dataContainer.end(); ++iter)
		{
			if (*(*iter) == *searchedData)
			{
				dataContainer.erase(iter);
				moveDataUp();

				return true;
			}
		}

		if (NodeNE != nullptr)
		{
			if (NodeNE->remove(searchedData))
				return true;
			if (NodeNW->remove(searchedData))
				return true;
			if (NodeSE->remove(searchedData))
				return true;
			if (NodeSW->remove(searchedData))
				return true;
		}
		throw std::exception("Error! Deleting Failed!");
	};
	bool removeFromThisNode(T& data)
	{
		std::scoped_lock<std::recursive_mutex> dataLock(dataAccess);

		//check if this node contains data
		for (auto iter = dataContainer.begin(); iter != dataContainer.end(); ++iter)
		{
			if (*(*iter) == *data)
			{
				dataContainer.erase(iter);
				moveDataUp();

				return true;
			}
		}
		return false;
	};


	bool subnodesEmpty()
	{
		if (NodeNE == nullptr)
			return true;

		if (NodeNE->isEmpty() && NodeNE->subnodesEmpty()
			&& NodeNW->isEmpty() && NodeNW->subnodesEmpty()
			&& NodeSE->isEmpty() && NodeSE->subnodesEmpty()
			&& NodeSW->isEmpty() && NodeSW->subnodesEmpty())
		{
			return true;
		}

		return false;
	};

	size_t getDataAvailableSize()
	{
		return bucketSize - dataContainer.size();
	}

	void deleteSubnodes()
	{
		if (NodeNE == nullptr)
			return;

		NodeNE->deleteSubnodes();
		NodeNW->deleteSubnodes();
		NodeSE->deleteSubnodes();
		NodeSW->deleteSubnodes();

		NodeNE.reset();
		NodeNW.reset();
		NodeSE.reset();
		NodeSW.reset();
	};

	bool isEmpty()
	{
		return dataContainer.empty();
	}

	void moveDataUp() {
		if (parentNode != nullptr)
		{
			auto iter = dataContainer.begin();
			size_t parentAvailableSize = parentNode->getDataAvailableSize();
			size_t i = 0;

			std::scoped_lock<std::recursive_mutex> dataLock(dataAccess);
			//while there is space at parent
			while (i < parentAvailableSize)
			{
				//try to insert data into parent node
				if (iter == dataContainer.end())
					break;
				try
				{
					if (parentNode->insertIntoThisNode(*iter))
					{
						//if sucessful then remove data from this node
						(*iter)->containingNode = parentNode;
						iter = dataContainer.erase(iter);
					}
					else
					{
						throw std::exception("Error moving data into parent node!");
					}
				}
				catch (std::exception x)
				{
					std::cout << x.what() << std::endl;
					std::abort();
				}

				++i;
			}
		}
		//if there is no space in this node, then return
		if (getDataAvailableSize() == 0)
			return;
		//if there is space left then move data from subnodes
		if (NodeNE != nullptr)
		{
			NodeNE->moveDataUp();
			NodeNW->moveDataUp();
			NodeSE->moveDataUp();
			NodeSW->moveDataUp();

			if (subnodesEmpty())
				deleteSubnodes();
		}
	};

	bool checkIfNodeChanged(T& searchedData)
	{
		if (boundaries.ContainsPoint(searchedData->position))
			return false;

		std::unique_lock<std::recursive_mutex> dataLock(dataAccess);

		for (auto iter = this->dataContainer.begin(); iter != this->dataContainer.end(); ++iter)
		{
			if (*(*iter) == *searchedData)
			{
				this->dataContainer.erase(iter);
				break;
			}
		}
		dataLock.unlock();

		updatePositionInTree(searchedData);

		moveDataUp();

		return true;
	};

	void updatePositionInTree(T& searchedData)
	{
		if (insert(searchedData))
			return;

		try
		{
			if (parentNode == nullptr)
				throw std::exception("ERROR! Point isn't contained by the tree!");
			else
				parentNode->updatePositionInTree(searchedData);
		}
		catch (std::exception exce)
		{
			std::cout << exce.what() << std::endl;
			std::abort();
		}
	};

	void GetVertexArray(std::vector<sf::VertexArray>& output) const
	{
		std::scoped_lock(dataAccess);
		if (NodeNE == nullptr)
		{
			output.emplace_back(vertex);
		}
		else
		{
			if (NodeNE != nullptr)
				NodeNE->GetVertexArray(output);

			if (NodeNW != nullptr)
				NodeNW->GetVertexArray(output);

			if (NodeSE != nullptr)
				NodeSE->GetVertexArray(output);

			if (NodeSW != nullptr)
				NodeSW->GetVertexArray(output);
		}
	};


public:

	unsigned int uDepth;

	size_t bucketSize;

	sf::VertexArray vertex;

	std::vector<T> dataContainer;

	AABB boundaries;

	Quadtree* parentNode;

	std::shared_ptr<Quadtree> NodeNE;
	std::shared_ptr<Quadtree> NodeNW;
	std::shared_ptr<Quadtree> NodeSE;
	std::shared_ptr<Quadtree> NodeSW;
private:
	std::recursive_mutex dataAccess;
};