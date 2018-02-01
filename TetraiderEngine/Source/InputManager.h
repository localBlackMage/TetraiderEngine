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

#define MAX_CONTROLLERS 4

enum class MOUSEBTN {
	MOUSE_BTN_LEFT,
	MOUSE_BTN_RIGHT,
	
	MOUSE_NUM_SCANCODES
};
enum class XBOX_SCANCODE {
	XBOX_DPAD_UP,
	XBOX_DPAD_DOWN,
	XBOX_DPAD_RIGHT,
	XBOX_DPAD_LEFT,
	XBOX_BTN_BACK,
	XBOX_BTN_START,
	XBOX_BTN_LEFT_SHOULDER,
	XBOX_BTN_RIGHT_SHOULDER,
	XBOX_BTN_A,
	XBOX_BTN_B,
	XBOX_BTN_X,
	XBOX_BTN_Y,
	XBOX_BTN_LEFT_STICK,
	XBOX_BTN_RIGHT_STICK,

	XBOX_NUM_SCANCODES
};

struct GameControllerState {
	// Button States
	bool prevUp, Up;
	bool prevDown, Down;
	bool prevRight, Right;
	bool prevLeft, Left;
	bool prevBack, Back;
	bool prevStart, Start;
	bool prevLeftShoulder, LeftShoulder;
	bool prevRightShoulder, RightShoulder;
	bool prevA, A;
	bool prevB, B;
	bool prevX, X;
	bool prevY, Y;
	bool prevLeftStick, LeftStick;
	bool prevRightStick, RightStick;
	// Axis States
	Sint16 prevLeftStickX, StickLeftX;
	Sint16 prevRightStickY, StickRightY;

};

class InputManager
{
private:
	// keyboard state
	Uint8 *mPreviousKeyStates;
	Uint8 *mCurrentKeyStates;
	// mouse state
	bool prevLeftMouse, leftMouse;
	bool prevRightMouse, rightMouse;
	int mousePosX, mousePosY;
	int mousePosRelX, mousePosRelY;
	// gamecontroller state
	GameControllerState mGameControllerStates;
	//SDL_GameController *mGameController;


public:
	InputManager();
	~InputManager();
	InputManager(const InputManager &) = delete;
	void operator=(const InputManager &) = delete;

	void Update();
	// Keyboard Input
	bool IsKeyPressed(const SDL_Scancode);
	bool IsKeyTriggered(const SDL_Scancode);
	bool IsKeyReleased(const SDL_Scancode);
	// Mouse Input
	bool IsMouseButtonPressed(const MOUSEBTN);
	bool IsMouseButtonTriggered(const MOUSEBTN);
	bool IsMouseButtonReleased(const MOUSEBTN);
	int MousePosX(void);
	int MousePosY(void);
	int MousePosRelX(void);
	int MousePosRelY(void);
	// XBOX 360 Controller Input
	bool IsKeyPressed(const XBOX_SCANCODE);
	bool IsKeyTriggered(const XBOX_SCANCODE);
	bool IsKeyReleased(const XBOX_SCANCODE);
};

#endif