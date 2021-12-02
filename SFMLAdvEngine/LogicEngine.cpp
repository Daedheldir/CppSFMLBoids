#include "LogicEngine.h"



dh::LogicEngine::LogicEngine(dh::GameDataRef m_gameData, float fMaxUpdatesPerSecond) : m_gameData(m_gameData)
{
	this->fMaxUpdatesPerSecond = fMaxUpdatesPerSecond;
}


dh::LogicEngine::~LogicEngine()
{
	if (logicThreadRunning)
		dispose();
}

void dh::LogicEngine::dispose()
{
	logicThreadRunning = false;
	if (m_updateThread.joinable()) {
		m_updateThread.join();
		std::cout << "Logic thread joined." << std::endl;
	}
	else {
		std::cout << "Logic thread not joinable!" << std::endl;
	}
}

void dh::LogicEngine::startLogicThread(const std::function<void()>& updateFunc)
{
	logicThreadRunning = true;
	m_updateThread = std::thread(&dh::LogicEngine::m_logicThread, this, updateFunc);
}

float dh::LogicEngine::GetUpdateElapsedTime() const
{
	return fUpdateElapsedTime;
}

void dh::LogicEngine::m_logicThread(const std::function<void()>& updateFunc)
{
	std::cout << "\tLogic thread started." << std::endl;

	while (this->logicThreadRunning)
	{
		m_updateLogicClock();
		updateFunc();
	}
}

void dh::LogicEngine::m_updateLogicClock()
{
	double elapsedTimeDiff = 0.0;

	//elapsed time update
	this->updateTime = this->updateClk.restart();
	this->fUpdateElapsedTime = this->updateTime.asSeconds();

	//fUpdateCounter += fUpdateElapsedTime;
	//if (this->fUpdateCounter >= 0.2f) {
	//	this->fUpdateCounter -= 0.2f;
	//}

	//if (this->fUpdateElapsedTime >= 0.2f) {
	//	this->fUpdateElapsedTime = 0.2f;
	//}

	//lock the updates per second
	if (this->fUpdateElapsedTime < fMaxUpdatesPerSecond) {

		elapsedTimeDiff = fMaxUpdatesPerSecond - this->fUpdateElapsedTime;

		this->fUpdateElapsedTime = static_cast<float>(fMaxUpdatesPerSecond);
		std::this_thread::sleep_for(std::chrono::duration<float>(elapsedTimeDiff));
	}
}
