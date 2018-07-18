/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>


Controller::Controller() :
	Agent(ComponentType::C_Controller)
{
}

Controller::~Controller() {}

void Controller::Deactivate() {
	Agent::Deactivate(); 
}

void Controller::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	Agent::Update(dt);
}

void Controller::LateUpdate(float dt) {}

void Controller::Serialize(const json& j) {
	Agent::Serialize(j["AgentData"]);
}

void Controller::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EVENT_INPUT_MOVE: {
			InputAxisAndMouseData* data = pEvent->Data<InputAxisAndMouseData>();
			m_pTransform->MoveAlongLookAt(data->m_dir);
			m_pTransform->RotateX(data->m_mouse.x);
			m_pTransform->RotateY(data->m_mouse.y);
			break;
		}
	}

	Agent::HandleEvent(pEvent);
}

void Controller::LateInitialize() {
	Agent::LateInitialize();
	TETRA_EVENTS.Subscribe(EVENT_INPUT_MOVE, this);
}
