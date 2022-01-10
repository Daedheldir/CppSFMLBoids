#pragma once
#include "BoidFlock.h"
#include "GPColor.h"
#include "FunctorBase.h"
#include "AlignmentBehaviour.h"
#include "CohesionBehaviour.h"
#include "SeparationBehaviour.h"
#include "RandomMovementBehaviour.h"

#include <vector>
#include <functional>
#include <Windows.h>

/*
1. Bit - wise AND : X & Y(or R)
2. Bit - wise OR : X | Y(or R)
3. Bit - wise XOR : X ^ Y(or R)
4. NOT : !X
5. Left - shift : X << 2 (multiplied by 2)
6. Right - shift : X >> 2 (divided by 2)
7. ADD : X + Y(or R)
8. SUB : X – Y(or R)
9. MUL : X * Y(or R)
10. DIV : X / Y(or R) if Y > 0
11. SIN: sin(X) * 255
12. COS : cos(X) * 255
13. TAN : tan(X) * 255
*/

//deep blue
//uint8_t red = static_cast<uint8_t>(static_cast<int>(posX / posY / r) % 256);
//uint8_t green = static_cast<uint8_t>(static_cast<int>(posX / posY * r) % 256);
//uint8_t blue = static_cast<uint8_t>(static_cast<int>(posX * posY / r) % 256);

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
			threadPool[i] = std::move(std::thread(&GPPopulationController::UpdatePopulations, this, i + 1));

			DWORD_PTR dw = SetThreadAffinityMask(threadPool[i].native_handle(), DWORD_PTR(1) << ((2 + (i * 2)) % std::thread::hardware_concurrency()));
			if (dw == 0)
			{
				DWORD dwErr = GetLastError();
				std::cerr << "SetThreadAffinityMask failed, GLE=" << dwErr << '\n';
			}
		}

		//job for this thread
		UpdatePopulations(0);

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

			//save evaluation iteration images
			for (int i = 0; i < threadPool.size(); ++i) {
				threadPool[i] = std::move(std::thread(&GPPopulationController::SaveEvaluationImage, this, i + 1, ""));
				DWORD_PTR dw = SetThreadAffinityMask(threadPool[i].native_handle(), DWORD_PTR(1) << ((1 + i) % std::thread::hardware_concurrency()));
				if (dw == 0)
				{
					DWORD dwErr = GetLastError();
					std::cerr << "SetThreadAffinityMask failed, GLE=" << dwErr << '\n';
				}
			}
			SaveEvaluationImage(0);
			for (auto& thread : threadPool)
			{
				if (thread.joinable())
					thread.join();
			}

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
			UpdatePopulations(i);

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
	void UpdatePopulations(const unsigned int flockIndex);
	void EvaluatePopulations(const unsigned int flockIndex);
	void SaveEvaluationImage(const unsigned int flockIndex, std::string additionalFileText = "") const;
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

	const float boidDiscardPercentage = 0.8f; //discarding worst boids
	std::vector<std::vector<std::pair<unsigned int, unsigned long int>>> populationBoidScores;	//flocks -> boids -> pair of boid index in vector and boid score
	//population related vecs
	std::vector<int> populationScores;
	std::vector<sf::Image> populationCanvases;
	std::vector<BoidFlock> populations;

	std::vector<std::thread> threadPool;
};
