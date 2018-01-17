/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.h
Purpose: Loads Meshes and other commonly used game resources into
memory and manages their instances as well as returns references to them.
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ResourceManager
{
private:
	ResourceManager();
	~ResourceManager();

public:
	ResourceManager(const ResourceManager &) = delete;
	void operator=(const ResourceManager &) = delete;

	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}
};

#endif