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

#pragma once
#ifndef FRAME_RATE_MANAGER_H
#define FRAME_RATE_MANAGER_H

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::milliseconds Millisecond;
typedef std::chrono::duration<float> FloatSecond;

class FrameRateManager
{
private:
	unsigned int m_maxFrameRate;
	float m_ticksPerFrame;
	TimePoint m_tickStart, m_tickEnd;
	float m_frameTime;
	float m_totalElapsedTime;
	float m_secondCounter;

public:
	FrameRateManager(unsigned int maxFrameRate = 60);
	~FrameRateManager();
	FrameRateManager(const FrameRateManager &) = delete;
	void operator=(const FrameRateManager &) = delete;
	void SetMaxFrameRate(unsigned int maxFrameRate = 60);
	void FrameStart();
	void FrameEnd();
	void ResetElapsedTime();
	float GetMaxFrameRate();
	float GetElapsedTime(); // total elapsed time from level start in seconds
	float GetFrameTime(); // in seconds
};

#endif