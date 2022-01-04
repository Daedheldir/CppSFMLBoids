#include "GPPopulationController.h"
#include <numeric>
#include <filesystem>
#include <algorithm>

GPPopulationController::GPPopulationController(sf::Image& refImage, const size_t populationsSize, const size_t flockSize, const unsigned int iterationsBetweenEvaluation, const unsigned int totalIterations) :
	refImage{ refImage },
	iterationsBetweenEvaluation{ iterationsBetweenEvaluation },
	totalIterations{ totalIterations }
{
	CreatePopulations(populationsSize, flockSize);

	//clear folder for evaluation images
	for (const auto& entry : std::filesystem::directory_iterator("../Data/Evaluations/")) {
		std::filesystem::remove_all(entry.path());
	}

	threadPool.resize(populationsSize - 1);
}

GPPopulationController::~GPPopulationController()
{
	for (int i = 0; i < populationCanvases.size(); ++i) {
		SaveEvaluationImage(i, "Final_Canvas");
	}
}

void GPPopulationController::CreatePopulations(const size_t populationsSize, const size_t flockSize)
{
	populations.clear();
	populations.reserve(populationsSize);
	populationColors.resize(populationsSize);
	populationAvailableFunctors.resize(populationsSize);
	populationCanvases.resize(populationsSize);
	populationScores.resize(populationsSize);
	populationBoidScores.resize(populationsSize);

	for (int i = 0; i < populationsSize; ++i) {
		populationColors[i].resize(flockSize);

		std::vector<FunctorBase::FunctorTypes> availableElements;
		for (int j = 0; j < FunctorBase::FunctorTypesCount; ++j) {
			availableElements.push_back(static_cast<FunctorBase::FunctorTypes>(j));
		}
		std::vector<int> selectedElements;
		for (int j = 0; j < 8; ++j) {
			int randVal = rand() % availableElements.size();

			if (std::find(selectedElements.begin(), selectedElements.end(), randVal) != selectedElements.end()) {
				//if element was found then try again
				--j;
				continue;
			}
			selectedElements.push_back(randVal);
			continue;
		}
		for (int j = 0; j < selectedElements.size(); ++j) {
			populationAvailableFunctors[i].push_back(static_cast<FunctorBase::FunctorTypes>(selectedElements[j]));
		}

		//		populationAvailableFunctors[i] = { {
		//				FunctorBase::FunctorTypes::Addition,
		//				FunctorBase::FunctorTypes::Multiplication,
		//				FunctorBase::FunctorTypes::Division
		//} };

				//boid flocks
		populations.emplace_back(flockSize, std::map<FlockBehaviourTypes, std::shared_ptr<FlockBehaviour>>
		{
			{
				{ FlockBehaviourTypes::Alignment, std::shared_ptr<FlockBehaviour>{new AlignmentBehaviour{ 1.0f }} },
				{ FlockBehaviourTypes::Cohesion, std::shared_ptr<FlockBehaviour>{new CohesionBehaviour{0.1f}} },
				{ FlockBehaviourTypes::Separation, std::shared_ptr<FlockBehaviour>{new SeparationBehaviour{1.5f} } },
				{ FlockBehaviourTypes::RandomMovement, std::shared_ptr<FlockBehaviour>{new RandomMovementBehaviour{0.5f}} }
			}
		},
			refImage.getSize() - sf::Vector2u{ 1, 1 }
		);

		populationCanvases[i].create(refImage.getSize().x, refImage.getSize().y);

		//initialize scores and assign boid colors
		populationBoidScores[i].reserve(flockSize);
		for (int j = 0; j < flockSize; ++j) {
			//scores
			populationBoidScores[i].emplace_back(std::pair{ j, 0 });

			//colors
			populationColors[i][j].SetPosition(populations[i].boidsDataArr[j].position);
			populations[i].boidsDataArr[j].color = populationColors[i][j].GetColor();
		}
	}
}

const BoidFlock& GPPopulationController::GetBestPopulation() const
{
	return populations[bestPopulationIndex];
}

void GPPopulationController::UpdatePopulations(const unsigned int flockIndex)
{
	int i = flockIndex;
	populations[i].MoveBoids();

	//update colors and decide whether to deposit
	for (int j = 0; j < populations[i].boidsDataArr.size(); ++j) {
		BoidAgentData& boid = populations[i].boidsDataArr[j];

		//update boid color
		populationColors[i][j].SetPosition(boid.position);
		boid.color = populationColors[i][j].GetColor();

		//compare colors with ref image
		sf::Color imageColor = refImage.getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));
		sf::Color currentColor = populationCanvases[i].getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));

		int currentDiff = std::abs(imageColor.r - currentColor.r) + std::abs(imageColor.g - currentColor.g) + std::abs(imageColor.b - currentColor.b);
		int boidDiff = std::abs(imageColor.r - boid.color.r) + std::abs(imageColor.g - boid.color.g) + std::abs(imageColor.b - boid.color.b);

		if (boidDiff < currentDiff) {
			//deposit boid color
			//for drawing on render texture boid alpha to 255 else to 0
			boid.color.a = 255;
			populationCanvases[i].setPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)), boid.color);
			populationBoidScores[i][j].second += 1;
		}
		else {
			boid.color.a = 0;
		}
		populations[i].boidsVerticesArr[j].color = boid.color;
	}
}

void GPPopulationController::EvaluatePopulations(const unsigned int flockIndex)
{
	int i = flockIndex;

	//sort population scores
	std::sort(populationBoidScores[i].begin(), populationBoidScores[i].end(), [](auto& a, auto& b)->bool {return a.second > b.second; });

	//calculate population scores
	int sum = 0;
	for (auto& pair : populationBoidScores[i]) {
		sum += pair.second;
	}

	populationScores[i] = sum;

	//evolve spawn new boids in place of discarded ones

	auto& boidsDataArr = populations[i].boidsDataArr;
	const unsigned int discardedBoidsPivot = static_cast<unsigned int>(std::round(boidDiscardPercentage * (populations[i].boidsDataArr.size() - 1)));

	std::vector<unsigned int> availableParentsIndices(boidsDataArr.size() - discardedBoidsPivot);

	for (unsigned int j = 0; j < populationBoidScores[i].size() - discardedBoidsPivot; ++j) {
		availableParentsIndices[j] = (populationBoidScores[i][j].first);
	}

	//for each discarded boid select a random parent from rest of population
	for (int j = discardedBoidsPivot; j < populationBoidScores[i].size(); ++j) {
		//using populationBoidsScores to select boids
		unsigned int discardedBoidIndex = populationBoidScores[i][j].first;
		unsigned int vectorRowToDeleteIndex = static_cast<unsigned int>(rand() % availableParentsIndices.size());
		unsigned int randParentIndex = availableParentsIndices[vectorRowToDeleteIndex];

		availableParentsIndices.erase(availableParentsIndices.begin() + vectorRowToDeleteIndex);

		//evolve discarded boid color
		populationColors[i][discardedBoidIndex].Evolve(populationColors[i][randParentIndex], populationAvailableFunctors[i]);

		//"spawn" boid at its parent position
		boidsDataArr[discardedBoidIndex].position = boidsDataArr[randParentIndex].position;
		//set boid color to invisible so it isn't visible until its new color is calculated
		boidsDataArr[discardedBoidIndex].color = sf::Color::Transparent;
	}

	//reset all boid scores for new evolution iteration
	std::for_each(populationBoidScores[i].begin(), populationBoidScores[i].end(), [](auto& pair)->void {pair.second = 0; });
}

void GPPopulationController::SaveEvaluationImage(const unsigned int flockIndex, std::string additionalFileText) const
{
	sf::Image outImg;
	const sf::Image& currentImg = populationCanvases[flockIndex];

	outImg.create(currentImg.getSize().x + refImage.getSize().x, currentImg.getSize().y > refImage.getSize().y ? currentImg.getSize().y : refImage.getSize().y);
	outImg.copy(refImage, 0, 0);
	outImg.copy(currentImg, refImage.getSize().x, 0);

	outImg.saveToFile(
		"../Data/Evaluations/Canvas_"
		+ std::to_string(flockIndex)
		+ "_eval" + std::to_string(evaluationsCounter)
		+ "_iter" + std::to_string(iterationsCounter)
		+ additionalFileText
		+ ".jpg");
}