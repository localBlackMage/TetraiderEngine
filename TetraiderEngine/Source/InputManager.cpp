#include <SDL.h>
#include <string.h>
#include <iostream>
#include "InputManager.h"



InputManager::InputManager()
	:	prevLeftMouse(false), leftMouse(false), 
		prevRightMouse(false), rightMouse(false) {
	// initialize InputManager
	mPreviousKeyStates = new Uint8[SDL_NUM_SCANCODES];
	mCurrentKeyStates = new Uint8[SDL_NUM_SCANCODES];
	memset(mPreviousKeyStates, 0, SDL_NUM_SCANCODES * sizeof(Uint8));
	memset(mCurrentKeyStates, 0, SDL_NUM_SCANCODES * sizeof(Uint8));

	// initialize for SDL_GameController
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
		printf( "ERROR: SDL_Init() with SDL_INIT_JOYSTICK flag failed!\n");
	}
	// open connected controller	
	//mGameController = SDL_GameControllerOpen(0);


	
}
InputManager::~InputManager() {
	// delete key states
	delete mCurrentKeyStates;
	delete mPreviousKeyStates;			
	//SDL_GameControllerClose(mGameController);
}

void InputManager::Update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				break;
		}
	}

	// Update mouse position
	int mouseTempPosX = mousePosX;
	int mouseTempPosY = mousePosY;
	Uint32 mouseState = SDL_GetMouseState(&mousePosX, &mousePosY);
	mousePosRelX = mousePosX - mouseTempPosX;
	mousePosRelY = mousePosY - mouseTempPosY;

	// Update previous mouse states
	prevLeftMouse = leftMouse;
	prevRightMouse = rightMouse;

	// Update current mouse states
	leftMouse = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) == 1;
	rightMouse = (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) == 4;
	
	// update PreviousKeyStates
	memcpy(mPreviousKeyStates, mCurrentKeyStates, SDL_NUM_SCANCODES * sizeof(Uint8));
	// get new KeyStates
	int fetchedNum = 0;
	const Uint8 *currentKeyStates = SDL_GetKeyboardState(&fetchedNum);
	// update CurrentKeyStates (only copy what is fetched)
	memcpy(mCurrentKeyStates, currentKeyStates, fetchedNum * sizeof(Uint8));

	// Update Current GameController States
	// TODO: get all other btn states and update prev, etc.
	SDL_GameController *mGameController;
	mGameController = SDL_GameControllerOpen(0);
	mGameControllerStates.Up = SDL_GameControllerGetButton(mGameController, SDL_CONTROLLER_BUTTON_DPAD_UP);
	mGameControllerStates.Down = SDL_GameControllerGetButton(mGameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	mGameControllerStates.Left = SDL_GameControllerGetButton(mGameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	mGameControllerStates.Right = SDL_GameControllerGetButton(mGameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

}

bool InputManager::IsKeyPressed(const SDL_Scancode scancode) {
	return mCurrentKeyStates[scancode];
}

bool InputManager::IsKeyTriggered(const SDL_Scancode scancode) {
	return (mCurrentKeyStates[scancode] == 1 && mPreviousKeyStates[scancode] == 0);
}

bool InputManager::IsKeyReleased(const SDL_Scancode scancode) {
	return (mCurrentKeyStates[scancode] == 0 && mPreviousKeyStates[scancode] == 1);
}

bool InputManager::IsMouseButtonPressed(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (leftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (rightMouse);

	return false;
}

bool InputManager::IsMouseButtonTriggered(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (!prevLeftMouse && leftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (!prevRightMouse && rightMouse);

	return false;
}


bool InputManager::IsMouseButtonReleased(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (prevLeftMouse && !leftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (prevRightMouse && !rightMouse);

	return false;
}

int InputManager::MousePosX() { return mousePosX; }
int InputManager::MousePosY() { return mousePosY; }
int InputManager::MousePosRelX() { return mousePosRelX; }
int InputManager::MousePosRelY() { return mousePosRelY; }


bool InputManager::IsKeyPressed(const XBOX_SCANCODE btn) {
	switch (btn) {
	case XBOX_SCANCODE::XBOX_DPAD_UP:
		return mGameControllerStates.Up;
		break;
	case XBOX_SCANCODE::XBOX_DPAD_DOWN:
		return mGameControllerStates.Down;
		break;

	case XBOX_SCANCODE::XBOX_DPAD_LEFT:
		return mGameControllerStates.Left;
		break;
	case XBOX_SCANCODE::XBOX_DPAD_RIGHT:
		return mGameControllerStates.Right;
		break;
	}
	return false;
}
bool InputManager::IsKeyTriggered(const XBOX_SCANCODE btn) {
	return false;
}
bool InputManager::IsKeyReleased(const XBOX_SCANCODE btn) {
	return false;
}