#include "FrameRateManager.h"

#define MIN_FRAME_TIME 0.01666666666666666666666666666667

FrameRateManager::FrameRateManager(unsigned int maxFrameRate) :
	m_maxFrameRate(maxFrameRate)
{}

FrameRateManager::~FrameRateManager() {}
