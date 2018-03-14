//#include "FrameRateManager.h"
//#include "TetraiderAPI.h"
//#include <SDL.h>
//#include <chrono>
//#include <stdint.h>
//#include <iostream>

#include <Stdafx.h>

#define MIN_FRAME_TIME 0.016667f

FrameRateManager::FrameRateManager(unsigned int maxFrameRate) :
	m_secondCounter(0.f)
{
	if (maxFrameRate == 0) {
		m_maxFrameRate = UINT16_MAX;
	}
	else { m_maxFrameRate = maxFrameRate;  }
	m_ticksPerFrame = 1000 / m_maxFrameRate;
	//m_frameTime = MIN_FRAME_TIME;
	m_totalElapsedTime = 0.0f;
}

FrameRateManager::~FrameRateManager() {}

void FrameRateManager::SetMaxFrameRate(unsigned int maxFrameRate)
{
	if (maxFrameRate == 0) {
		m_maxFrameRate = UINT16_MAX;
	}
	else { m_maxFrameRate = maxFrameRate; }
	m_ticksPerFrame = 1000 / m_maxFrameRate;
}

void FrameRateManager::FrameStart() {
	m_tickStart = Clock::now();
}

void FrameRateManager::FrameEnd() {
	m_tickEnd = Clock::now();
	Millisecond duration = std::chrono::duration_cast<Millisecond>(m_tickEnd - m_tickStart);
	while (duration.count() < m_ticksPerFrame) {
		m_tickEnd = Clock::now();
	}

	if (TETRA_GAME_STATE.IsDebugPause()) {
		m_frameTime = GetMaxFrameRate();
		m_totalElapsedTime += m_frameTime;
	}
	else {
		FloatSecond fsec = m_tickEnd - m_tickStart;
		m_frameTime = float(fsec.count()); //  / 1000.0f;
		std::cout << "frametime: " << m_frameTime << std::endl;
		m_totalElapsedTime += m_frameTime;

		m_secondCounter += m_frameTime;
		float fps = 1 / m_frameTime;
		if (fps < 40.0f) {
			std::cout << "FPS dropped to: " << fps << std::endl;
		}
	}

	//if (m_secondCounter >= 0.1f) {
	//	TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_FPS_UPDATE, &FPSData(1/m_frameTime)));
	//	m_secondCounter = 0.f;
	//}
}

void FrameRateManager::ResetElapsedTime() {
	m_totalElapsedTime = 0.0f;
}

float FrameRateManager::GetElapsedTime() {
	return m_totalElapsedTime;
}

float FrameRateManager::GetFrameTime() {
	return m_frameTime;
}

float FrameRateManager::GetMaxFrameRate() {
	return MIN_FRAME_TIME;
}