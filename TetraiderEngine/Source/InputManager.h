/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.h
Purpose: Provides all input related funcionality.
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define MAX_CONTROLLERS 4

enum class MOUSEBTN {
	MOUSE_BTN_LEFT,
	MOUSE_BTN_RIGHT,
	
	MOUSE_NUM_SCANCODES
};
enum XBOX_SCANCODE {
	XBOX_BTN_A,
	XBOX_BTN_B,
	XBOX_BTN_X,
	XBOX_BTN_Y,
	XBOX_BTN_BACK,
	XBOX_BTN_GUIDE,
	XBOX_BTN_START,
	XBOX_BTN_LEFT_STICK,
	XBOX_BTN_RIGHT_STICK,
	XBOX_BTN_LEFT_SHOULDER,
	XBOX_BTN_RIGHT_SHOULDER,
	XBOX_DPAD_UP,
	XBOX_DPAD_DOWN,
	XBOX_DPAD_LEFT,
	XBOX_DPAD_RIGHT,

	XBOX_NUM_SCANCODES
};

enum InputType {
	InputType_Button,
	InputType_Axis,
	InputType_MousePosOrJoystick,

	InputType_NUM
};

enum JoystickAnalogueType {
	JoystickAnalogue_Left,
	JoystickAnalogue_Right,

	JoystickAnalogue_NUM
};

struct InputCommandInfo {
	InputCommandInfo(EventType _event,
		InputType _inputType,
		bool _isMouse,
		bool _isJoystick,
		SDL_Scancode _keyboardKey,
		SDL_Scancode _keyboardPosX,
		SDL_Scancode _keyboardNegX,
		SDL_Scancode _keyboardPosY,
		SDL_Scancode _keyboardNegY,
		JoystickAnalogueType _analogue,
		MOUSEBTN _mouseBtn,
		XBOX_SCANCODE _xboxKey) :
		m_event(_event),
		m_inputType(_inputType),
		m_isMouse(_isMouse),
		m_isJoystick(_isJoystick),
		m_keyboardKey(_keyboardKey),
		m_keyboardKeyPosX(_keyboardPosX),
		m_keyboardKeyNegX(_keyboardNegX),
		m_keyboardKeyPosY(_keyboardPosY),
		m_keyboardKeyNegY(_keyboardNegY),
		m_analogue(_analogue),
		m_mouseBtn(_mouseBtn),
		m_xboxKey(_xboxKey) {}
	EventType m_event;
	InputType m_inputType;
	bool m_isMouse;
	bool m_isJoystick;
	SDL_Scancode m_keyboardKey;
	SDL_Scancode m_keyboardKeyPosX;
	SDL_Scancode m_keyboardKeyNegX;
	SDL_Scancode m_keyboardKeyPosY;
	SDL_Scancode m_keyboardKeyNegY;
	JoystickAnalogueType m_analogue;
	MOUSEBTN m_mouseBtn;
	XBOX_SCANCODE m_xboxKey;
};

class InputManager: public Subscriber
{
private:
	// keyboard state
	Uint8 *m_PreviousKeyStates;
	Uint8 *m_CurrentKeyStates;
	// mouse state
	bool m_PrevLeftMouse, m_LeftMouse;
	bool m_PrevRightMouse, m_RightMouse;
	int m_MousePosX, m_MousePosY;
	int m_MousePosRelX, m_MousePosRelY;
	bool m_isJoystickControlsActive;
	// Game controller States
	Uint8  *m_CurrentButtonStates;
	Uint8  *m_PreviousButtonStates;
	// Axis States
	Sint16 m_StickRightX, m_StickLeftX;
	Sint16 m_StickRightY, m_StickLeftY;

	std::vector<InputCommandInfo*> m_inputCommands;

	void FireEvents();
public:
	InputManager();
	~InputManager();
	InputManager(const InputManager &) = delete;
	void operator=(const InputManager &) = delete;

	void Update();
	void HandleEvent(Event* pEvent);
	void Initialize(const json& j);
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
	Sint16 GetLeftAxisX();
	Sint16 GetLeftAxisY();
	Sint16 GetRightAxisX();
	Sint16 GetRightAxisY();
};

#endif