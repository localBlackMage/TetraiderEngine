/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.h
Purpose: Provides all input related funcionality.
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SDL_keycode.h"

class InputManager
{
private:
	InputManager();
	~InputManager();

public:
	InputManager(const InputManager &) = delete;
	void operator=(const InputManager &) = delete;

	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}
};

#endif