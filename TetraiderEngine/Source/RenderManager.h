/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: RenderManager.h
Purpose: Provides all rendering functionality.
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

class RenderManager
{
private:
	RenderManager();
	~RenderManager();

public:
	RenderManager(const RenderManager &) = delete;
	void operator=(const RenderManager &) = delete;

	static RenderManager& GetInstance()
	{
		static RenderManager instance;
		return instance;
	}
};

#endif