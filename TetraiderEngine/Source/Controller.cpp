#include "Controller.h"
#include "Transform.h"
#include "InputManager.h"

#include <iostream>

Controller::Controller() :
	Component(ComponentType::Controller),
	m_speed (0.0f)
{
}

Controller::~Controller() {}

void Controller::Update(float dt) {
	InputManager& inputMngr = InputManager::GetInstance();

	Vector3D posOffset;

	if (inputMngr.IsKeyPressed(SDL_SCANCODE_D))
		posOffset.x += m_speed*dt;
	if (inputMngr.IsKeyPressed(SDL_SCANCODE_A))
		posOffset.x -= m_speed*dt;
	if (inputMngr.IsKeyPressed(SDL_SCANCODE_W))
		posOffset.y += m_speed*dt;
	if (inputMngr.IsKeyPressed(SDL_SCANCODE_S))
		posOffset.y -= m_speed*dt;

	m_pTransform->SetPosition(posOffset + m_pTransform->GetPosition());
}

void Controller::Serialize(json j) {
	m_speed = ParseFloat(j, "speed");
}

void Controller::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
		else {
			printf("No Game Object found. Controller component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Controller component failed to operate.\n");
			return;
		}
	}
}