/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.h
Purpose: Provides all input related funcionality.
Language: C++
Project: GAM541
Author: Holden Profit, Hyoyup Chung
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SDL_keycode.h"

enum class MouseBtn {
	MOUSEBTN_LEFT,
	MOUSEBTN_RIGHT,

	NONE
};

class InputManager
{
private:
	Uint8 *mPreviousKeyStates;
	Uint8 *mCurrentKeyStates;
	bool prevLeftMouse, leftMouse;
	bool prevRightMouse, rightMouse;
	int mousePosX, mousePosY;
	int mousePosRelX, mousePosRelY;

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

	void Update();
	// Keyboard Input
	bool IsKeyPressed(const SDL_Scancode);
	bool IsKeyTriggered(const SDL_Scancode);
	bool IsKeyReleased(const SDL_Scancode);
	// Mouse Input
	bool IsMouseButtonPressed(const MouseBtn);
	bool IsMouseButtonTriggered(const MouseBtn);
	bool IsMouseButtonReleased(const MouseBtn);
	int MousePosX();
	int MousePosY();
	int MousePosRelX();
	int MousePosRelY();
};

#endif