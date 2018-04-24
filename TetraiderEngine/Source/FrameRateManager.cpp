/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#define MIN_FRAME_TIME 0.016667f

FrameRateManager::FrameRateManager(unsigned int maxFrameRate) :
	m_secondCounter(0.f),
	m_frameTime(0.0166667f)
{
	if (maxFrameRate == 0) {
		m_maxFrameRate = UINT16_MAX;
	}
	else { m_maxFrameRate = maxFrameRate;  }
	m_ticksPerFrame = 1.0f / (float)m_maxFrameRate;
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
	m_ticksPerFrame = 1.0f / (float)m_maxFrameRate;
}

void FrameRateManager::FrameStart() {
	m_tickStart = Clock::now();
}

void FrameRateManager::FrameEnd() {
	m_tickEnd = Clock::now();
	Millisecond duration = std::chrono::duration_cast<Millisecond>(m_tickEnd - m_tickStart);
	while (duration.count() < m_ticksPerFrame) {
		duration = std::chrono::duration_cast<Millisecond>(m_tickEnd - m_tickStart);
		m_tickEnd = Clock::now();
	}

	if (TETRA_GAME_STATE.IsDebugPause()) {
		m_frameTime = GetMaxFrameRate();
		m_totalElapsedTime += m_frameTime;
	}
	else {
		auto dur = m_tickEnd - m_tickStart;
		FloatSecond fsec = std::chrono::duration_cast<FloatSecond>(dur);
		m_frameTime = float(fsec.count()); //  / 1000.0f;
		m_totalElapsedTime += m_frameTime;

		m_secondCounter += m_frameTime;
		float fps = 1.0f / m_frameTime;
		if (fps < 40.0f) {
			//std::cout << "FPS dropped to: " << fps << std::endl;
		}
	}

	//if (m_secondCounter >= 0.1f) {
	//	TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_FPS_UPDATE, &FloatData(1/m_frameTime)));
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