/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FrameRateManager.h
Purpose: Provides timer functionality to the engine. Singleton class.
Language: C++
Project: GAM541
Author: Holden Profit, Hyoyup Chung
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#ifndef FRAME_RATE_MANAGER_H
#define FRAME_RATE_MANAGER_H

class FrameRateManager
{
private:
	unsigned int m_maxFrameRate, m_ticksPerFrame;
	unsigned int m_tickStart, m_tickEnd;
	float m_frameTime;

	FrameRateManager(unsigned int maxFrameRate);
	~FrameRateManager();
public:
	FrameRateManager(const FrameRateManager &) = delete;
	void operator=(const FrameRateManager &) = delete;

	static FrameRateManager& GetInstance(unsigned int maxFrameRate = 60)
	{
		static FrameRateManager instance(maxFrameRate);
		return instance;
	}

	void FrameStart();
	void FrameEnd();
	float GetFrameTime(); // in seconds
};

#endif