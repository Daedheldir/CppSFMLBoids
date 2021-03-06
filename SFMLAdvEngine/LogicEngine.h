#pragma once
#include <thread>
#include <functional>

#include "GameData.h"

namespace dh {
	class LogicEngine
	{
	public:
		LogicEngine(dh::GameDataRef m_gameData, float fMaxUpdatesPerSecond);
		~LogicEngine();

		void dispose();

		void startLogicThread(const std::function<void()>& updateFunc);
		float GetUpdateElapsedTime() const;

	private:
		void m_logicThread(const std::function<void()>& updateFunc);

		void m_updateLogicClock();

	private:
		dh::GameDataRef m_gameData;

		bool logicThreadRunning = false;
		std::thread m_updateThread;

		float	fUpdateElapsedTime = 1.0f;

		sf::Clock	updateClk;
		sf::Time	updateTime;

		float	fMaxUpdatesPerSecond = 1.0f;	//max logic updates per second equal to 1.0f / (int) maxUpdatesPerSecond
		//float	fUpdateCounter;
	};

}