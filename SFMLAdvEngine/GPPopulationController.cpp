#include "GPPopulationController.h"
#include <numeric>
#include <filesystem>
#include <algorithm>

#include "DebugFlags.h"
#include "FunctorFactory.h"

GPPopulationController::GPPopulationController(sf::Image& refImage, const size_t populationsSize, const size_t flockSize, const unsigned int iterationsBetweenEvaluation, const unsigned int iterationsBetweenImageSave, const unsigned int totalIterations) :
	refImage{ refImage },
	iterationsBetweenEvaluation{ iterationsBetweenEvaluation },
	iterationsBetweenImageSave{ iterationsBetweenImageSave },
	totalIterations{ totalIterations }
{
	CreatePopulations(populationsSize, flockSize);

	//if folder for evaluation images doesn't exist then create it
	if (!std::filesystem::exists("../Data/Evaluations/"))
		std::filesystem::create_directories("../Data/Evaluations/");

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
		std::vector<FunctorBase::FunctorTypes> availableElements;
		for (int j = 0; j < FunctorBase::FunctorTypesCount; ++j) {
			availableElements.push_back(static_cast<FunctorBase::FunctorTypes>(j));
		}
		std::vector<FunctorBase::FunctorTypes> selectedElements;
		for (int j = 0; j < 3; ++j) {
			if (availableElements.size() <= 0) break;

			int randVal = rand() % availableElements.size();

			selectedElements.push_back(availableElements[randVal]);
			availableElements.erase(availableElements.begin() + randVal);
		}
		for (int j = 0; j < selectedElements.size(); ++j) {
			populationAvailableFunctors[i].push_back(selectedElements[j]);
		}

		populationPerceptionFunctors.push_back(FunctorFactory::CreateFunctor(static_cast<FunctorBase::FunctorTypes>(rand() % FunctorBase::FunctorTypesCount)));

		populationColors[i].resize(flockSize);

		std::shared_ptr<FunctorBase> redFunctor = FunctorFactory::CreateFunctor(populationAvailableFunctors[i][rand() % populationAvailableFunctors[i].size()]);
		std::shared_ptr<FunctorBase> greenFunctor = FunctorFactory::CreateFunctor(populationAvailableFunctors[i][rand() % populationAvailableFunctors[i].size()]);
		std::shared_ptr<FunctorBase> blueFunctor = FunctorFactory::CreateFunctor(populationAvailableFunctors[i][rand() % populationAvailableFunctors[i].size()]);

		for (int j = 0; j < flockSize; ++j) {
			populationColors[i][j].Initialize(redFunctor, greenFunctor, blueFunctor);
		}
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

void GPPopulationController::EvaluatePopulations(const unsigned int flockIndex)
{
	int i = flockIndex;

	//sort population scores
	std::sort(populationBoidScores[i].begin(), populationBoidScores[i].end(), [](auto& a, auto& b)->bool {return a.second > b.second; });

	//calculate population scores
	int sum = 0;
	std::for_each(populationBoidScores[i].begin(), populationBoidScores[i].end(), [&](auto& pair)->void {sum += pair.second; });

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

	//if image height is bigger then width then position images horizontally
	if (currentImg.getSize().x <= currentImg.getSize().y) {
		outImg.create(currentImg.getSize().x + refImage.getSize().x, currentImg.getSize().y > refImage.getSize().y ? currentImg.getSize().y : refImage.getSize().y);
		outImg.copy(refImage, 0, 0);
		outImg.copy(currentImg, refImage.getSize().x, 0);
	}
	else {
		outImg.create(currentImg.getSize().x > refImage.getSize().x ? currentImg.getSize().x : refImage.getSize().x, currentImg.getSize().y + refImage.getSize().y);
		outImg.copy(refImage, 0, 0);
		outImg.copy(currentImg, 0, refImage.getSize().y);
	}
	std::string functorsStr = "";
	if (DebugFlags::POPULATION_UPDATE_METHOD == DebugFlags::PopulationUpdateModifiers::WARPED_PERCEPTION) {
		functorsStr += "_WP_" + (*populationPerceptionFunctors[flockIndex]).GetName();
	}
	for (const auto& func : populationAvailableFunctors[flockIndex])
	{
		functorsStr += "_" + (*FunctorFactory::CreateFunctor(func)).GetName();
	}

	outImg.saveToFile(
		"../Data/Evaluations/Canvas_"
		+ std::to_string(flockIndex)
		+ "_eval" + std::to_string(evaluationsCounter)
		+ "_iter" + std::to_string(iterationsCounter)
		+ functorsStr
		+ additionalFileText
		+ ".jpg");
}

void GPPopulationController::SetCanvasColor(const sf::Vector2u& pixelPos, const unsigned int flockIndex, const sf::Color& newColor)
{
	int i = flockIndex;

	//drawing with a bigger brush
	populationCanvases[i].setPixel(pixelPos.x, pixelPos.y, newColor);
	if (pixelPos.x > 0) {
		sf::Color currentColor = populationCanvases[i].getPixel(pixelPos.x - 1, pixelPos.y);
		populationCanvases[i].setPixel(pixelPos.x - 1, pixelPos.y,
			{
				static_cast<uint8_t>(std::lerp(currentColor.r, newColor.r, DebugFlags::BRUSH_SIZE_LERP)),
				static_cast<uint8_t>(std::lerp(currentColor.g, newColor.g, DebugFlags::BRUSH_SIZE_LERP)),
				static_cast<uint8_t>(std::lerp(currentColor.b, newColor.b, DebugFlags::BRUSH_SIZE_LERP)),
			});
	}
	if (pixelPos.x < refImage.getSize().x - 1) {
		sf::Color currentColor = populationCanvases[i].getPixel(pixelPos.x + 1, pixelPos.y);
		populationCanvases[i].setPixel(pixelPos.x + 1, pixelPos.y,
			{
				static_cast<uint8_t>(std::lerp(currentColor.r, newColor.r, DebugFlags::BRUSH_SIZE_LERP)),
				static_cast<uint8_t>(std::lerp(currentColor.g, newColor.g, DebugFlags::BRUSH_SIZE_LERP)),
				static_cast<uint8_t>(std::lerp(currentColor.b, newColor.b, DebugFlags::BRUSH_SIZE_LERP)),
			});
	}
	if (pixelPos.y > 0) {
		sf::Color currentColor = populationCanvases[i].getPixel(pixelPos.x, pixelPos.y - 1);
		populationCanvases[i].setPixel(pixelPos.x, pixelPos.y - 1,
			{
				static_cast<uint8_t>(std::lerp(currentColor.r, newColor.r, DebugFlags::BRUSH_SIZE_LERP)),
				static_cast<uint8_t>(std::lerp(currentColor.g, newColor.g, DebugFlags::BRUSH_SIZE_LERP)),
				static_cast<uint8_t>(std::lerp(currentColor.b, newColor.b, DebugFlags::BRUSH_SIZE_LERP)),
			});
	}
	if (pixelPos.y < refImage.getSize().y - 1) {
		sf::Color currentColor = populationCanvases[i].getPixel(pixelPos.x, pixelPos.y + 1);
		populationCanvases[i].setPixel(pixelPos.x, pixelPos.y + 1,
			{
				static_cast<uint8_t>(std::lerp(currentColor.r, newColor.r, DebugFlags::BRUSH_SIZE_LERP)),
				static_cast<uint8_t>(std::lerp(currentColor.g, newColor.g, DebugFlags::BRUSH_SIZE_LERP)),
				static_cast<uint8_t>(std::lerp(currentColor.b, newColor.b, DebugFlags::BRUSH_SIZE_LERP)),
			});
	}
}