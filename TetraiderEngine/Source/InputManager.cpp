#include <SDL.h>
#include <string.h>
#include "InputManager.h"



InputManager::InputManager()
	:	prevLeftMouse(false), leftMouse(false), 
		prevRightMouse(false), rightMouse(false) {
	// initialize InputManager
	mPreviousKeyStates = new Uint8[SDL_NUM_SCANCODES];
	mCurrentKeyStates = new Uint8[SDL_NUM_SCANCODES];
	memset(mPreviousKeyStates, 0, SDL_NUM_SCANCODES * sizeof(Uint8));
	memset(mCurrentKeyStates, 0, SDL_NUM_SCANCODES * sizeof(Uint8));
}
InputManager::~InputManager() {
	// delete key states
	delete mCurrentKeyStates;
	delete mPreviousKeyStates;
}

void InputManager::Update() {
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

bool InputManager::IsMouseButtonPressed(MouseBtn btn) {
	if (btn == MouseBtn::MOUSEBTN_LEFT)
		return (leftMouse);
	else if (btn == MouseBtn::MOUSEBTN_RIGHT)
		return (rightMouse);

	return false;
}

bool InputManager::IsMouseButtonTriggered(MouseBtn btn) {
	if (btn == MouseBtn::MOUSEBTN_LEFT)
		return (!prevLeftMouse && leftMouse);
	else if (btn == MouseBtn::MOUSEBTN_RIGHT)
		return (!prevRightMouse && rightMouse);

	return false;
}


bool InputManager::IsMouseButtonReleased(MouseBtn btn) {
	if (btn == MouseBtn::MOUSEBTN_LEFT)
		return (prevLeftMouse && !leftMouse);
	else if (btn == MouseBtn::MOUSEBTN_RIGHT)
		return (prevRightMouse && !rightMouse);

	return false;
}

int InputManager::MousePosX() { return mousePosX; }
int InputManager::MousePosY() { return mousePosY; }
int InputManager::MousePosRelX() { return mousePosRelX; }
int InputManager::MousePosRelY() { return mousePosRelY; }