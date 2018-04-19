/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#define JoystickDeadZone 5500

InputManager::InputManager()
	:	m_PrevLeftMouse(false), m_LeftMouse(false), 
		m_PrevRightMouse(false), m_RightMouse(false), m_isJoystickControlsActive(false) {
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

	TETRA_EVENTS.Subscribe(EVENT_INPUT_TOGGLEJOYSTICK, this);
}

InputManager::~InputManager() {
	// delete key states
	delete m_CurrentKeyStates;
	delete m_PreviousKeyStates;			
	//SDL_GameControllerClose(mGameController);
	delete m_CurrentButtonStates;
	delete m_PreviousButtonStates;

	for (auto command : m_inputCommands) {
		delete command;
	}

	m_inputCommands.clear();
}

void InputManager::Initialize(const json& j) {
	int size = j.size();
	for (int i = 0; i < size; ++i) {
		m_inputCommands.push_back(new InputCommandInfo(
			static_cast<EventType>(ParseInt(j[i], "EventType")),
			static_cast<InputType>(ParseInt(j[i], "InputType")),
			ParseBool(j[i], "isMouse"),
			ParseBool(j[i], "isJoystick"),
			static_cast<SDL_Scancode>(ParseInt(j[i], "KeyBoardKey")),
			static_cast<SDL_Scancode>(ParseInt(j[i], "KeyBoardKeyPosX")),
			static_cast<SDL_Scancode>(ParseInt(j[i], "KeyBoardKeyNegX")),
			static_cast<SDL_Scancode>(ParseInt(j[i], "KeyBoardKeyPosY")),
			static_cast<SDL_Scancode>(ParseInt(j[i], "KeyBoardKeyNegY")),
			static_cast<JoystickAnalogueType>(ParseInt(j[i], "AnalogueStick")),
			static_cast<MOUSEBTN>(ParseInt(j[i], "MouseBtn")),
			static_cast<XBOX_SCANCODE>(ParseInt(j[i], "XboxKey"))
		));
	}
}

void InputManager::Update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		TETRA_IMGUI.HandleSDLEvents(event);
		switch (event.type) {
			case SDL_QUIT:
				TETRA_EVENTS.BroadcastEvent(&Event(EventType::EVENT_WINDOW_CLOSED));
				break;
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
					{
						TETRA_RENDERER.SetWindowDimensions(event.window.data1, event.window.data2);
						break;
					}
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					case SDL_WINDOWEVENT_MINIMIZED:
					{
						if(!TETRA_GAME_STATE.IsGamePaused())
							TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_INPUT_PAUSEGAME, &InputButtonData(false, true, false)));
						break;
					}
				}
				break;
			}
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

	// update CurrentButtonStates // TODO Optimization: Ignore all this if joystick is not connected
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

	FireEvents();
}

void InputManager::FireEvents() {
	for (auto command : m_inputCommands) {
		switch (command->m_inputType) {
			case InputType_Button: {
				bool isTrigger = false, isRelease = false, isPressed = false;
				if (command->m_isJoystick && command->m_isMouse) {
					isTrigger = (IsMouseButtonTriggered(command->m_mouseBtn) || IsKeyTriggered(command->m_xboxKey));
					isRelease = (IsMouseButtonReleased(command->m_mouseBtn) || IsKeyReleased(command->m_xboxKey));
					isPressed = (IsMouseButtonPressed(command->m_mouseBtn) || IsKeyPressed(command->m_xboxKey));
				}
				else if (command->m_isJoystick) {
					isTrigger = (IsKeyTriggered(command->m_keyboardKey) || IsKeyTriggered(command->m_xboxKey));
					isRelease = (IsKeyReleased(command->m_keyboardKey) || IsKeyReleased(command->m_xboxKey));
					isPressed = (IsKeyPressed(command->m_keyboardKey) || IsKeyPressed(command->m_xboxKey));
				}
				else if (command->m_isMouse) {
					isTrigger = IsMouseButtonTriggered(command->m_mouseBtn);
					isRelease = IsMouseButtonReleased(command->m_mouseBtn);
					isPressed = IsMouseButtonPressed(command->m_mouseBtn);
				}
				else {
					isTrigger = IsKeyTriggered(command->m_keyboardKey);
					isRelease = IsKeyReleased(command->m_keyboardKey);
					isPressed = IsKeyPressed(command->m_keyboardKey);
				}
				if (isTrigger || isRelease || isPressed) {
					TETRA_EVENTS.BroadcastEventToSubscribers(&Event(command->m_event, &InputButtonData(isPressed, isTrigger, isRelease)));
				}
				break;
			}
			case InputType_Axis: {
				float xAxis = 0, yAxis = 0;

				if (command->m_isJoystick && command->m_analogue == JoystickAnalogue_Left) {
					if (abs(GetLeftAxisX()) > JoystickDeadZone)  xAxis += GetLeftAxisX();
					if (abs(GetLeftAxisY()) > JoystickDeadZone) yAxis -= GetLeftAxisY();
				}
				else if(command->m_isJoystick) {
					if (abs(GetRightAxisX()) > JoystickDeadZone) xAxis += GetRightAxisX();
					if (abs(GetRightAxisY()) > JoystickDeadZone) yAxis -= GetRightAxisX();
				}

				if (IsKeyPressed(command->m_keyboardKeyPosX)) xAxis += 1;
				if (IsKeyPressed(command->m_keyboardKeyNegX)) xAxis -= 1;
				if (IsKeyPressed(command->m_keyboardKeyPosY)) yAxis += 1;
				if (IsKeyPressed(command->m_keyboardKeyNegY)) yAxis -= 1;

				Vector3D axisDir(xAxis, yAxis, 0);
				axisDir.Normalize();
				TETRA_EVENTS.BroadcastEventToSubscribers(&Event(command->m_event, &InputAxisData(axisDir)));
				break;
			}
			// This case is unique for our game regarding mouse (Check else statement below)
			case InputType_MousePosOrJoystick: {
				if (command->m_isJoystick && m_isJoystickControlsActive) {
					float xAxis = 0, yAxis = 0;

					if (command->m_analogue == JoystickAnalogue_Left) {
						if (abs(GetLeftAxisX()) > JoystickDeadZone) xAxis += GetLeftAxisX();
						if (abs(GetLeftAxisY()) > JoystickDeadZone) yAxis -= GetLeftAxisY();
					}
					else if (command->m_isJoystick) {
						if (abs(GetRightAxisX()) > JoystickDeadZone) xAxis += GetRightAxisX();
						if (abs(GetRightAxisY()) > JoystickDeadZone) yAxis -= GetRightAxisY();
					}

					Vector3D axisDir(xAxis, yAxis, 0);
					axisDir.Normalize();
					TETRA_EVENTS.BroadcastEventToSubscribers(&Event(command->m_event, &InputAxisData(axisDir)));
				}
				else {
					// This is the hardcoded part:
					if(TETRA_GAME_OBJECTS.GetPlayer())
						TETRA_EVENTS.BroadcastEventToSubscribers(&Event(command->m_event, &InputAxisData(Agent::GetDirectionFromPlayerToMouse())));
				}
				break;
			}
		}
	}
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

void InputManager::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EVENT_INPUT_TOGGLEJOYSTICK: {
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isTrigger) m_isJoystickControlsActive = !m_isJoystickControlsActive;
			break;
		}
	}
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