#include "InputManager.h"
#include "TetraiderAPI.h"

#include <SDL.h>
#include <string.h>
#include <iostream>

#include "TetraiderAPI.h"

InputManager::InputManager()
	:	m_PrevLeftMouse(false), m_LeftMouse(false), 
		m_PrevRightMouse(false), m_RightMouse(false) {
	// initialize keyboard states
	m_PreviousKeyStates = new Uint8[SDL_NUM_SCANCODES];
	m_CurrentKeyStates = new Uint8[SDL_NUM_SCANCODES];
	memset(m_PreviousKeyStates, 0, SDL_NUM_SCANCODES * sizeof(Uint8));
	memset(m_CurrentKeyStates, 0, SDL_NUM_SCANCODES * sizeof(Uint8));
	// initialize gamecontroller button states
	m_PreviousButtonStates = new Uint8[XBOX_NUM_SCANCODES];
	m_CurrentButtonStates = new Uint8[XBOX_NUM_SCANCODES];
	memset(m_PreviousKeyStates, 0, XBOX_NUM_SCANCODES * sizeof(Uint8));
	memset(m_CurrentKeyStates, 0, XBOX_NUM_SCANCODES * sizeof(Uint8));
	m_StickRightX, m_StickLeftX = 0;
	m_StickRightY, m_StickLeftY = 0;

	// initialize for SDL_GameController
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
		printf( "ERROR: SDL_Init() with SDL_INIT_JOYSTICK flag failed!\n");
	}
}

InputManager::~InputManager() {
	// delete key states
	delete m_CurrentKeyStates;
	delete m_PreviousKeyStates;			
	//SDL_GameControllerClose(mGameController);
	delete m_CurrentButtonStates;
	delete m_PreviousButtonStates;
}

void InputManager::Update() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type) {
			case SDL_QUIT:
				TETRA_EVENTS.BroadcastEvent(&Event(EventType::WINDOW_CLOSED));
				break;
		}
	}

	// Update mouse position
	int mouseTempPosX = m_MousePosX;
	int mouseTempPosY = m_MousePosY;
	Uint32 mouseState = SDL_GetMouseState(&m_MousePosX, &m_MousePosY);
	m_MousePosRelX = m_MousePosX - mouseTempPosX;
	m_MousePosRelY = m_MousePosY - mouseTempPosY;

	// Update previous mouse states
	m_PrevLeftMouse = m_LeftMouse;
	m_PrevRightMouse = m_RightMouse;

	// Update current mouse states
	m_LeftMouse = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) == 1;
	m_RightMouse = (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) == 4;
	
	// update PreviousKeyStates
	memcpy(m_PreviousKeyStates, m_CurrentKeyStates, SDL_NUM_SCANCODES * sizeof(Uint8));
	memcpy(m_PreviousButtonStates, m_CurrentButtonStates, XBOX_NUM_SCANCODES * sizeof(Uint8));

	// get new KeyStates
	int fetchedNum = 0;
	const Uint8 *currentKeyStates = SDL_GetKeyboardState(&fetchedNum);
	// update CurrentKeyStates (only copy what is fetched)
	memcpy(m_CurrentKeyStates, currentKeyStates, fetchedNum * sizeof(Uint8));

	// update CurrentButtonStates
	SDL_GameController *GameController;
	GameController = SDL_GameControllerOpen(0);
	m_CurrentButtonStates[XBOX_BTN_A] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_A);
	m_CurrentButtonStates[XBOX_BTN_B] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_B);
	m_CurrentButtonStates[XBOX_BTN_X] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_X);
	m_CurrentButtonStates[XBOX_BTN_Y] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_Y);
	m_CurrentButtonStates[XBOX_BTN_BACK] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_BACK);
	m_CurrentButtonStates[XBOX_BTN_GUIDE] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_GUIDE);
	m_CurrentButtonStates[XBOX_BTN_START] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_START);
	m_CurrentButtonStates[XBOX_BTN_LEFT_STICK] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_LEFTSTICK);
	m_CurrentButtonStates[XBOX_BTN_RIGHT_STICK] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
	m_CurrentButtonStates[XBOX_BTN_LEFT_SHOULDER] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	m_CurrentButtonStates[XBOX_BTN_RIGHT_SHOULDER] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	m_CurrentButtonStates[XBOX_DPAD_UP] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_UP);
	m_CurrentButtonStates[XBOX_DPAD_DOWN] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	m_CurrentButtonStates[XBOX_DPAD_LEFT] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	m_CurrentButtonStates[XBOX_DPAD_RIGHT] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	m_StickLeftX = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTX);
	m_StickLeftY = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTY);
	m_StickRightX = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTX);
	m_StickRightY = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTY);
}

bool InputManager::IsKeyPressed(const SDL_Scancode scancode) {
	return m_CurrentKeyStates[scancode];
}

bool InputManager::IsKeyTriggered(const SDL_Scancode scancode) {
	return (m_CurrentKeyStates[scancode] == 1 && m_PreviousKeyStates[scancode] == 0);
}

bool InputManager::IsKeyReleased(const SDL_Scancode scancode) {
	return (m_CurrentKeyStates[scancode] == 0 && m_PreviousKeyStates[scancode] == 1);
}

bool InputManager::IsMouseButtonPressed(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (m_LeftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (m_RightMouse);

	return false;
}

bool InputManager::IsMouseButtonTriggered(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (!m_PrevLeftMouse && m_LeftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (!m_PrevRightMouse && m_RightMouse);

	return false;
}


bool InputManager::IsMouseButtonReleased(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (m_PrevLeftMouse && !m_LeftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (m_PrevRightMouse && !m_RightMouse);

	return false;
}

int InputManager::MousePosX() { return m_MousePosX; }
int InputManager::MousePosY() { return m_MousePosY; }
int InputManager::MousePosRelX() { return m_MousePosRelX; }
int InputManager::MousePosRelY() { return m_MousePosRelY; }


bool InputManager::IsKeyPressed(const XBOX_SCANCODE btn) {
	return m_CurrentButtonStates[btn];
}
bool InputManager::IsKeyTriggered(const XBOX_SCANCODE btn) {
	return (m_CurrentButtonStates[btn] == 1 && m_PreviousButtonStates[btn] == 0);
}
bool InputManager::IsKeyReleased(const XBOX_SCANCODE btn) {
	return (m_CurrentButtonStates[btn] == 0 && m_PreviousButtonStates[btn] == 1);
}

Sint16 InputManager::GetLeftAxisX() {
	return m_StickLeftX;
}
Sint16 InputManager::GetLeftAxisY() {
	return m_StickLeftY;
}
Sint16 InputManager::GetRightAxisX() {
	return m_StickRightX;
}
Sint16 InputManager::GetRightAxisY() {
	return m_StickRightY;
}