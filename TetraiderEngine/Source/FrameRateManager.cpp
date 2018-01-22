#include "FrameRateManager.h"

#include <SDL.h>
#include <stdint.h>
#include <iostream>

#define MIN_FRAME_TIME 0.01666666666666666666666666666667f

FrameRateManager::FrameRateManager(unsigned int maxFrameRate) :
	m_maxFrameRate(maxFrameRate) {
	if (maxFrameRate == 0) {
		m_maxFrameRate = UINT16_MAX;
	}
	else { m_maxFrameRate = maxFrameRate;  }
	m_ticksPerFrame = 1000 / m_maxFrameRate;
	m_tickStart, m_tickEnd = 0;
	m_frameTime = MIN_FRAME_TIME;
	m_totalElapsedTime = 0.0f;
}

FrameRateManager::~FrameRateManager() {}

void FrameRateManager::FrameStart() {
	m_tickStart = SDL_GetTicks();
}

void FrameRateManager::FrameEnd() {
	m_tickEnd = SDL_GetTicks();
	while (m_tickEnd - m_tickStart < m_ticksPerFrame) {
		m_tickEnd = SDL_GetTicks();
	}
	m_frameTime = (float)(m_tickEnd - m_tickStart) / 1000.0f;
	m_totalElapsedTime += m_frameTime;
	//std::cout << m_frameTime<<"\n";
	//std::cout << m_totalElapsedTime << "\n";
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