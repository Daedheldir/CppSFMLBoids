#pragma once
#include "BoidFlock.h"
#include "GPParameters.h"
#include "GPColor.h"
#include "FunctorBase.h"
#include "AlignmentBehaviour.h"
#include "CohesionBehaviour.h"
#include "SeparationBehaviour.h"
#include "RandomMovementBehaviour.h"

#include <vector>
#include <functional>
#include <Windows.h>

class GPPopulationController
{
public:
	enum class Threading
	{
		Enabled,
		Disabled
	};

	GPPopulationController(sf::Image& refImage, const size_t populationsSize, const size_t flockSize, const unsigned int iterationsBetweenEvaluation, const unsigned int iterationsBetweenImageSave, const unsigned int totalIterations);
	~GPPopulationController();
	void CreatePopulations(const size_t populationsSize, const size_t flockSize);

	template<Threading T>
	void UpdateGP();

	template<>
	void UpdateGP<Threading::Enabled>()
	{
		++iterationsCounter;
		if (iterationsCounter > totalIterations) {
			simRunning = false;
			return;
		}

		for (int i = 0; i < threadPool.size(); ++i) {
			threadPool[i] = std::move(std::thread(&GPPopulationController::UpdatePopulations<GPParameters::POPULATION_UPDATE_METHOD>, this, i + 1));

			DWORD_PTR dw = SetThreadAffinityMask(threadPool[i].native_handle(), DWORD_PTR(1) << ((2 + (i * 2)) % std::thread::hardware_concurrency()));
			if (dw == 0)
			{
				DWORD dwErr = GetLastError();
				std::cerr << "SetThreadAffinityMask failed, GLE=" << dwErr << '\n';
			}
		}

		//job for this thread
		UpdatePopulations<GPParameters::POPULATION_UPDATE_METHOD>(0);

		for (auto& thread : threadPool)
		{
			if (thread.joinable())
				thread.join();
		}

		if (iterationsCounter % iterationsBetweenEvaluation == 0
			&& iterationsCounter < totalIterations)
		{
			for (int i = 0; i < populations.size(); ++i) {
				EvaluatePopulations(i);
			}
			++evaluationsCounter;

			////save evaluation iteration images
			//for (int i = 0; i < threadPool.size(); ++i) {
			//	threadPool[i] = std::move(std::thread(&GPPopulationController::SaveEvaluationImage, this, i + 1, ""));
			//	DWORD_PTR dw = SetThreadAffinityMask(threadPool[i].native_handle(), DWORD_PTR(1) << ((1 + i) % std::thread::hardware_concurrency()));
			//	if (dw == 0)
			//	{
			//		DWORD dwErr = GetLastError();
			//		std::cerr << "SetThreadAffinityMask failed, GLE=" << dwErr << '\n';
			//	}
			//}
			//SaveEvaluationImage(0);
			//for (auto& thread : threadPool)
			//{
			//	if (thread.joinable())
			//		thread.join();
			//}

			std::cout << "Evaluating Pop " << evaluationsCounter << std::endl;
		}
	};

	template<>
	void UpdateGP<Threading::Disabled>()
	{
		++iterationsCounter;
		if (iterationsCounter > totalIterations) {
			simRunning = false;
			return;
		}

		for (int i = 0; i < populations.size(); ++i)
			UpdatePopulations<GPParameters::POPULATION_UPDATE_METHOD>(i);

		if (iterationsCounter % iterationsBetweenEvaluation == 0
			&& iterationsCounter < totalIterations)
		{
			for (int i = 0; i < populations.size(); ++i)
				EvaluatePopulations(i);

			++evaluationsCounter;
			//save evaluation iteration images
			for (int i = 0; i < populationCanvases.size(); ++i) {
				SaveEvaluationImage(i);
			}
			std::cout << "Evaluating Pop " << evaluationsCounter << std::endl;
		}
	};

	const BoidFlock& GetBestPopulation() const;
private:
	template<GPParameters::PopulationUpdateModifiers T>
	void UpdatePopulations(const unsigned int flockIndex) {
		int i = flockIndex;
		populations[i].MoveBoids();

		//update colors and decide whether to deposit
		for (int j = 0; j < populations[i].boidsDataArr.size(); ++j) {
			BoidAgentData& boid = populations[i].boidsDataArr[j];

			//update boid color
			populationColors[i][j].SetPosition(boid.position);
			boid.color = populationColors[i][j].GetColor();
			boid.color.a = 0;

			//compare colors with ref image
			sf::Color imageColor = refImage.getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));
			sf::Color currentColor = populationCanvases[i].getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));

			if (imageColor.a <= 0) continue; //skip this pixel if it's transparent

			const int currentDiff = std::abs(static_cast<int>(imageColor.r) - static_cast<int>(currentColor.r))
				+ std::abs(static_cast<int>(imageColor.g) - static_cast<int>(currentColor.g))
				+ std::abs(static_cast<int>(imageColor.b) - static_cast<int>(currentColor.b));

			const int boidDiff = std::abs(static_cast<int>(imageColor.r) - static_cast<int>(boid.color.r))
				+ std::abs(static_cast<int>(imageColor.g) - static_cast<int>(boid.color.g))
				+ std::abs(static_cast<int>(imageColor.b) - static_cast<int>(boid.color.b));

			sf::Vector2u pixelPos{ static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)) };
			sf::Color newColor = populationCanvases[i].getPixel(pixelPos.x, pixelPos.y);

			if (boidDiff < currentDiff) {
				boid.color.a = 255;
				newColor = boid.color;
				populationBoidScores[i][j].second += boidDiff;
			}

			SetCanvasColor(pixelPos, flockIndex, newColor);
			populations[i].boidsVerticesArr[j].color = newColor;
		}

		//save iteration image
		if (iterationsCounter % iterationsBetweenImageSave == 0) {
			SaveEvaluationImage(flockIndex);
		}
	};
	template<>
	void UpdatePopulations<GPParameters::PopulationUpdateModifiers::DEFAULT_WITH_SEPARATE_COLORS_COMPARISON>(const unsigned int flockIndex)
	{
		int i = flockIndex;
		populations[i].MoveBoids();

		//update colors and decide whether to deposit
		for (int j = 0; j < populations[i].boidsDataArr.size(); ++j) {
			BoidAgentData& boid = populations[i].boidsDataArr[j];

			//update boid color
			populationColors[i][j].SetPosition(boid.position);
			boid.color = populationColors[i][j].GetColor();
			boid.color.a = 0;

			//compare colors with ref image
			sf::Color imageColor = refImage.getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));
			sf::Color currentColor = populationCanvases[i].getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));

			if (imageColor.a <= 0) continue; //skip this pixel if it's transparent

			sf::Vector3i currentSeparateDiff = { std::abs(imageColor.r - currentColor.r) , std::abs(imageColor.g - currentColor.g) , std::abs(imageColor.b - currentColor.b) };
			sf::Vector3i boidSeparateDiff = { std::abs(imageColor.r - boid.color.r) , std::abs(imageColor.g - boid.color.g) , std::abs(imageColor.b - boid.color.b) };

			sf::Vector2u pixelPos{ static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)) };
			sf::Color newColor = populationCanvases[i].getPixel(pixelPos.x, pixelPos.y);

			if (boidSeparateDiff.x < currentSeparateDiff.x
				&& boidSeparateDiff.y < currentSeparateDiff.y
				&& boidSeparateDiff.z < currentSeparateDiff.z) {
				boid.color.a = 255;
				newColor = boid.color;
				populationBoidScores[i][j].second += boidSeparateDiff.x + boidSeparateDiff.y + boidSeparateDiff.z;
			}

			SetCanvasColor(pixelPos, flockIndex, newColor);
			populations[i].boidsVerticesArr[j].color = newColor;
		}

		//save iteration image
		if (iterationsCounter % iterationsBetweenImageSave == 0) {
			SaveEvaluationImage(flockIndex);
		}
	};
	template<>
	void UpdatePopulations<GPParameters::PopulationUpdateModifiers::DEFAULT_WITH_WEIGHTED_COLORS_COMPARISON>(const unsigned int flockIndex)
	{
		int i = flockIndex;
		populations[i].MoveBoids();

		//update colors and decide whether to deposit
		for (int j = 0; j < populations[i].boidsDataArr.size(); ++j) {
			BoidAgentData& boid = populations[i].boidsDataArr[j];

			//update boid color
			populationColors[i][j].SetPosition(boid.position);
			boid.color = populationColors[i][j].GetColor();
			boid.color.a = 0;

			//compare colors with ref image
			sf::Color imageColor = refImage.getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));
			sf::Color currentColor = populationCanvases[i].getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));

			if (imageColor.a <= 0) continue; //skip this pixel if it's transparent

			float colorMax = std::max({ static_cast<float>(imageColor.r), static_cast<float>(imageColor.g), static_cast<float>(imageColor.b) });

			sf::Vector3f colorWeights
			{
				imageColor.r / colorMax,
				imageColor.g / colorMax,
				imageColor.b / colorMax
			};

			const int currentDiff = std::abs(static_cast<int>(static_cast<int>(imageColor.r) - static_cast<int>(currentColor.r)) * colorWeights.x)
				+ static_cast<int>(std::abs(static_cast<int>(imageColor.g) - static_cast<int>(currentColor.g)) * colorWeights.y)
				+ static_cast<int>(std::abs(static_cast<int>(imageColor.b) - static_cast<int>(currentColor.b)) * colorWeights.z);

			const int boidDiff = static_cast<int>(std::abs(static_cast<int>(imageColor.r) - static_cast<int>(boid.color.r)) * colorWeights.x)
				+ static_cast<int>(std::abs(static_cast<int>(imageColor.g) - static_cast<int>(boid.color.g)) * colorWeights.y)
				+ static_cast<int>(std::abs(static_cast<int>(imageColor.b) - static_cast<int>(boid.color.b)) * colorWeights.z);

			sf::Vector2u pixelPos{ static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)) };
			sf::Color newColor = populationCanvases[i].getPixel(pixelPos.x, pixelPos.y);

			if (boidDiff < currentDiff) {
				boid.color.a = 255;
				newColor = boid.color;
				populationBoidScores[i][j].second += std::abs(imageColor.r - boid.color.r)
					+ std::abs(imageColor.g - boid.color.g)
					+ std::abs(imageColor.b - boid.color.b);
			}

			SetCanvasColor(pixelPos, flockIndex, newColor);
			populations[i].boidsVerticesArr[j].color = newColor;
		}

		//save iteration image
		if (iterationsCounter % iterationsBetweenImageSave == 0) {
			SaveEvaluationImage(flockIndex);
		}
	};

	template<>
	void UpdatePopulations<GPParameters::PopulationUpdateModifiers::SEPARATE_COLORS>(const unsigned int flockIndex)
	{
		int i = flockIndex;
		populations[i].MoveBoids();

		//update colors and decide whether to deposit
		for (int j = 0; j < populations[i].boidsDataArr.size(); ++j) {
			BoidAgentData& boid = populations[i].boidsDataArr[j];

			//update boid color
			populationColors[i][j].SetPosition(boid.position);
			boid.color = populationColors[i][j].GetColor();
			boid.color.a = 0;

			//compare colors with ref image
			sf::Color imageColor = refImage.getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));
			sf::Color currentColor = populationCanvases[i].getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));

			if (imageColor.a <= 0) continue; //skip this pixel if it's transparent

			sf::Vector3i currentSeparateDiff = { std::abs(imageColor.r - currentColor.r) , std::abs(imageColor.g - currentColor.g) , std::abs(imageColor.b - currentColor.b) };
			sf::Vector3i boidSeparateDiff = { std::abs(imageColor.r - boid.color.r) , std::abs(imageColor.g - boid.color.g) , std::abs(imageColor.b - boid.color.b) };

			sf::Vector2u pixelPos{ static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)) };
			sf::Color newColor = populationCanvases[i].getPixel(pixelPos.x, pixelPos.y);

			if (boidSeparateDiff.x < currentSeparateDiff.x) {
				boid.color.a = 255;
				newColor.r = boid.color.r;
				populationBoidScores[i][j].second += boidSeparateDiff.x;
			}
			if (boidSeparateDiff.y < currentSeparateDiff.y) {
				boid.color.a = 255;
				newColor.g = boid.color.g;
				populationBoidScores[i][j].second += boidSeparateDiff.y;
			}
			if (boidSeparateDiff.z < currentSeparateDiff.z) {
				boid.color.a = 255;
				newColor.b = boid.color.b;
				populationBoidScores[i][j].second += boidSeparateDiff.z;
			}
			SetCanvasColor(pixelPos, flockIndex, newColor);
			populations[i].boidsVerticesArr[j].color = newColor;
		}

		//save iteration image
		if (iterationsCounter % iterationsBetweenImageSave == 0) {
			SaveEvaluationImage(flockIndex);
		}
	};
	template<>
	void UpdatePopulations<GPParameters::PopulationUpdateModifiers::WARPED_PERCEPTION>(const unsigned int flockIndex) {
		int i = flockIndex;
		populations[i].MoveBoids();

		//update colors and decide whether to deposit
		for (int j = 0; j < populations[i].boidsDataArr.size(); ++j) {
			BoidAgentData& boid = populations[i].boidsDataArr[j];

			//update boid color
			populationColors[i][j].SetPosition(boid.position);
			boid.color = populationColors[i][j].GetColor();
			boid.color.a = 0;

			//compare colors with ref image
			sf::Color imageColor = refImage.getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));
			sf::Color currentColor = populationCanvases[i].getPixel(static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)));

			if (imageColor.a <= 0) continue; //skip this pixel if it's transparent

			long currentPerceptionDiff = (*populationPerceptionFunctors[i])(std::abs(imageColor.r - currentColor.r), std::abs(imageColor.g - currentColor.g));
			currentPerceptionDiff = (*populationPerceptionFunctors[i])(currentPerceptionDiff, std::abs(imageColor.b - currentColor.b));
			long boidPerceptionDiff = (*populationPerceptionFunctors[i])(std::abs(imageColor.r - boid.color.r), std::abs(imageColor.g - boid.color.g));
			boidPerceptionDiff = (*populationPerceptionFunctors[i])(boidPerceptionDiff, std::abs(imageColor.b - boid.color.b));

			sf::Vector2u pixelPos{ static_cast<unsigned int>(std::floorl(boid.position.x)), static_cast<unsigned int>(std::floorl(boid.position.y)) };
			sf::Color newColor = populationCanvases[i].getPixel(pixelPos.x, pixelPos.y);

			if (boidPerceptionDiff - currentPerceptionDiff < 2) {
				boid.color.a = 255;
				newColor = boid.color;
				populationBoidScores[i][j].second += boidPerceptionDiff;
			}

			SetCanvasColor(pixelPos, flockIndex, newColor);
			populations[i].boidsVerticesArr[j].color = newColor;
		}

		//save iteration image
		if (iterationsCounter % iterationsBetweenImageSave == 0) {
			SaveEvaluationImage(flockIndex);
		}
	};

	void EvaluatePopulations(const unsigned int flockIndex);
	void SaveEvaluationImage(const unsigned int flockIndex, std::string additionalFileText = "") const;

private:
	void SetCanvasColor(const sf::Vector2u& pixelPos, const unsigned int flockIndex, const sf::Color& newColor);
public:
	bool simRunning = true;
	static constexpr Threading threading = Threading::Enabled;
private:
	unsigned int bestPopulationIndex = 0;
	unsigned int iterationsCounter = 0;
	unsigned int evaluationsCounter = 0;

	const unsigned int iterationsBetweenEvaluation;
	const unsigned int iterationsBetweenImageSave;
	const unsigned int totalIterations;

	const sf::Image& refImage;

	//boid related vecs
	std::vector<std::vector<GPColor>> populationColors;
	std::vector<std::vector <FunctorBase::FunctorTypes>> populationAvailableFunctors;

	std::vector<std::vector<std::pair<unsigned int, unsigned long int>>> populationBoidScores;	//flocks -> boids -> pair of boid index in vector and boid score
	//population related vecs
	std::vector<int> populationScores;
	std::vector<sf::Image> populationCanvases;
	std::vector<BoidFlock> populations;
	std::vector<std::shared_ptr<FunctorBase>> populationPerceptionFunctors;

	std::vector<std::thread> threadPool;
};
