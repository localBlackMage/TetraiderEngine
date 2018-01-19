#include "FrameRateManager.h"

#include <SDL.h>
#include <stdint.h>

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
}

float FrameRateManager::GetFrameTime() {
	return m_frameTime;
}