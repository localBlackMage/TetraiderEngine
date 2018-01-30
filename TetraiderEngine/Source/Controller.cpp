#include "Controller.h"
#include "Transform.h"
#include "Body.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "DebugManager.h"
#include "Health.h"
#include "../Source/Math/LineSegment2D.h"

#include <iostream>

Controller::Controller() :
	Agent(ComponentType::Controller)
{
}

Controller::~Controller() {}

void Controller::Update(float dt) {
	InputManager& inputMngr = InputManager::GetInstance();

	Vector3D moveDir;

	if (inputMngr.IsKeyPressed(SDL_SCANCODE_D))
		moveDir.x += 1;
	if (inputMngr.IsKeyPressed(SDL_SCANCODE_A))
		moveDir.x -= 1;
	if (inputMngr.IsKeyPressed(SDL_SCANCODE_W))
		moveDir.y += 1;
	if (inputMngr.IsKeyPressed(SDL_SCANCODE_S))
		moveDir.y -= 1;

	if (inputMngr.IsKeyTriggered(SDL_SCANCODE_Q)) {
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