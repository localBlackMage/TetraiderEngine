#include "GameObject.h"
#include "Controller.h"
#include "Health.h"
#include "TetraiderAPI.h"
#include <iostream>

Controller::Controller() :
	Agent(ComponentType::C_Controller)
{
}

Controller::~Controller() {}

void Controller::Update(float dt) {
	Vector3D moveDir;

	if (T_INPUT.IsKeyPressed(SDL_SCANCODE_D)|| T_INPUT.IsKeyPressed(XBOX_DPAD_RIGHT))
		moveDir.x += 1;
	if (T_INPUT.IsKeyPressed(SDL_SCANCODE_A) || T_INPUT.IsKeyPressed(XBOX_DPAD_LEFT))
		moveDir.x -= 1;
	if (T_INPUT.IsKeyPressed(SDL_SCANCODE_W) || T_INPUT.IsKeyPressed(XBOX_DPAD_UP))
		moveDir.y += 1;
	if (T_INPUT.IsKeyPressed(SDL_SCANCODE_S) || T_INPUT.IsKeyPressed(XBOX_DPAD_DOWN))
		moveDir.y -= 1;

	if (T_INPUT.IsKeyTriggered(SDL_SCANCODE_SPACE))
	{
		T_AUDIO.PlaySFX("../TetraiderEngine/Assets/SFX/pew.mp3", 0.8f);
		std::cout << "shots fired!\n";
	}
	if (T_INPUT.IsKeyTriggered(SDL_SCANCODE_P))
		T_AUDIO.TogglePause();

	if (T_INPUT.IsKeyTriggered(SDL_SCANCODE_Q) || T_INPUT.IsKeyTriggered(XBOX_BTN_LEFT_SHOULDER)){
		AddVelocity(Vector3D(-750, -750, 0));
		Health* pHealth = pGO->GetComponent<Health>(ComponentType::C_Health);
		pHealth->TakeDamage(10);
	}

	moveDir.Normalize();
	m_targetVelocity = moveDir * m_speed;
	Agent::Update(dt);

}

void Controller::Serialize(json j) {
	Agent::Serialize(j["AgentData"]);
}

void Controller::HandleEvent(Event* pEvent) {
	Agent::HandleEvent(pEvent);
}

void Controller::LateInitialize() {
	Agent::LateInitialize();
}