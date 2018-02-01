#include "Controller.h"
#include "Transform.h"
#include "Body.h"
#include "Health.h"
#include "Math/LineSegment2D.h"

#include <iostream>

Controller::Controller() :
	Agent(ComponentType::Controller)
{
}

Controller::~Controller() {}

void Controller::Update(float dt) {
	Vector3D moveDir;

	if (T_INPUT.IsKeyPressed(SDL_SCANCODE_D)||T_INPUT.IsKeyPressed(XBOX_SCANCODE::XBOX_DPAD_RIGHT))
		moveDir.x += 1;
	if (T_INPUT.IsKeyPressed(SDL_SCANCODE_A) || T_INPUT.IsKeyPressed(XBOX_SCANCODE::XBOX_DPAD_LEFT))
		moveDir.x -= 1;
	if (T_INPUT.IsKeyPressed(SDL_SCANCODE_W) || T_INPUT.IsKeyPressed(XBOX_SCANCODE::XBOX_DPAD_UP))
		moveDir.y += 1;
	if (T_INPUT.IsKeyPressed(SDL_SCANCODE_S) || T_INPUT.IsKeyPressed(XBOX_SCANCODE::XBOX_DPAD_DOWN))
		moveDir.y -= 1;

	if (T_INPUT.IsKeyTriggered(SDL_SCANCODE_Q)) {
		AddVelocity(Vector3D(-750, -750, 0));
		Health* pHealth = static_cast<Health*>(pGO->GetComponent(ComponentType::Health));
		pHealth->TakeDamage(10);
	}

	moveDir.Normalize();
	m_targetVelocity = moveDir*m_speed;
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