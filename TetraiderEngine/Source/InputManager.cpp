#include <SDL.h>
#include <string.h>
#include "InputManager.h"



InputManager::InputManager() {
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