#include "GPPopulationController.h"

GPPopulationController::GPPopulationController(sf::Image& refImage, const size_t populationsSize, const size_t flockSize, const unsigned int iterationsToRun) :
	refImage{ refImage },
	iterationsToRun{ iterationsToRun }
{
	CreatePopulations(populationsSize, flockSize);
}

GPPopulationController::~GPPopulationController()
{
	for (int i = 0; i < populationCanvases.size(); ++i) {
		populationCanvases[i].saveToFile("../Data/Canvas_" + std::to_string(i) + ".bmp");
	}
}

void GPPopulationController::CreatePopulations(const size_t populationsSize, const size_t flockSize)
{
	populations.clear();
	populations.reserve(populationsSize);
	populationCanvases.resize(populationsSize);

	for (int i = 0; i < populationsSize; ++i) {
		//boid flocks
		populations.emplace_back(flockSize, std::map<FlockBehaviourTypes, std::shared_ptr<FlockBehaviour>>
		{
			{
				{ FlockBehaviourTypes::Alignment, std::shared_ptr<FlockBehaviour>{new AlignmentBehaviour{ 1.0f }} },
				{ FlockBehaviourTypes::Cohesion, std::shared_ptr<FlockBehaviour>{new CohesionBehaviour{0.1f}} },
				{ FlockBehaviourTypes::Separation, std::shared_ptr<FlockBehaviour>{new SeparationBehaviour{1.5f} } },
				{ FlockBehaviourTypes::RandomMovement, std::shared_ptr<FlockBehaviour>{new RandomMovementBehaviour{0.5f}} }
			}
		}
		);
		//TODO: REMOVE HARDCODED VALUES
		populationCanvases[i].create(520, 520);
	}
}

void GPPopulationController::UpdatePopulations()
{
	for (int i = 0; i < populations.size(); ++i) {
		populations[i].MoveBoids();
		for (int j = 0; j < populations[i].boidsDataArr.size(); ++j) {
			BoidAgentData& boid = populations[i].boidsDataArr[j];
			//compare colors with ref image
			//TODO: GET RID OF RENDER TEXTURES, DRAW BOIDS TO IMAGE
			sf::Color imageColor = refImage.getPixel(std::floorl(boid.position.x), std::floorl(boid.position.y));
			sf::Color currentColor = populationCanvases[i].getPixel(std::floorl(boid.position.x), std::floorl(boid.position.y));

			int currentDiff = std::abs(imageColor.r - currentColor.r) + std::abs(imageColor.g - currentColor.g) + std::abs(imageColor.b - currentColor.b);
			int boidDiff = std::abs(imageColor.r - boid.color.r) + std::abs(imageColor.g - boid.color.g) + std::abs(imageColor.b - boid.color.b);

			if (boidDiff < currentDiff) {
				//deposit boid color
				//for drawing on render texture boid alpha to 1 else to 0
				boid.color.a = 255;
				populationCanvases[i].setPixel(std::floorl(boid.position.x), std::floorl(boid.position.y), boid.color);
			}
			else {
				boid.color.a = 0;
			}
			populations[i].boidsVerticesArr[j].color = boid.color;
		}
	}
}

const BoidFlock& GPPopulationController::GetBestPopulation()
{
	return populations[bestPopulationIndex];
}

sf::RenderTexture& GPPopulationController::GetBestPopulationRenderTexture()
{
	return populations[bestPopulationIndex].renderTexture;
}
