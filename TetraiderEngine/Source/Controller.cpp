#include "GameObject.h"
#include "Controller.h"
#include "Weapon.h"
#include "Health.h"
#include "Stamina.h"
#include "Transform.h"
#include "ParticleEmitter.h"
#include "TetraiderAPI.h"
#include <iostream>

Controller::Controller() :
	Agent(ComponentType::C_Controller), m_isGameControllerEnabled(true), m_flySpeed(0)
{
}

Controller::~Controller() {}

void Controller::Deactivate() {
	m_pWeapon = nullptr; 
	Agent::Deactivate(); 
}

void Controller::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;
	Agent::Update(dt);
}
void Controller::Serialize(const json& j) {
	Agent::Serialize(j["AgentData"]);
	m_flySpeed = ParseFloat(j, "flySpeed");
	m_pParticleEmitterGO = TETRA_GAME_OBJECTS.CreateGameObject(ParseString(j, "featherParticlePrefab"));
	m_pParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->DeactivateParticles();
}

void Controller::HandleEvent(Event* pEvent) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	Agent::HandleEvent(pEvent);

	switch (pEvent->Type()) {
		case EVENT_INPUT_MOVE: {
			InputAxisData* pAxisData = pEvent->Data<InputAxisData>();
			if(!m_isIgnoreHazards) m_targetVelocity = pAxisData->m_dir*m_speed;
			else {
				m_targetVelocity = pAxisData->m_dir*m_flySpeed;
				m_lookDirection = pAxisData->m_dir;
			}
			break;
		}
		case EVENT_INPUT_AIM: {
			if (!m_isIgnoreHazards) {
				InputAxisData* pAxisData = pEvent->Data<InputAxisData>();
				if (!pAxisData->m_dir.IsVectorZero()) m_lookDirection = pAxisData->m_dir;
			}
			break;
		}
		case EVENT_INPUT_FLY: {
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isPressed && m_pStamina->UseStamina(TETRA_FRAMERATE.GetFrameTime())) {
				m_isIgnoreHazards = true;
				m_pParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->ActivateParticles();
			}
			else {
				m_isIgnoreHazards = false;
				m_pParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->DeactivateParticles();
			}
			break;
		}
		case EVENT_INPUT_MELEE: {
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isPressed) m_pWeapon->UseAttack(0, m_lookDirection);
			break;
		}
		case EVENT_INPUT_RANGE: {
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isPressed) m_pWeapon->UseAttack(1, m_lookDirection);
			break;
		}
	}
}

void Controller::LateInitialize() {
	Agent::LateInitialize();
	if (m_pParticleEmitterGO) {
		m_pParticleEmitterGO->SetParent(pGO);
	}
	if(!m_pWeapon) {
		if (pGO)
			m_pWeapon = pGO->GetComponent<Weapon>(ComponentType::C_Weapon);
		else {
			printf("No Game Object found. Controller component failed to operate.\n");
			return;
		}

		if (!m_pWeapon) {
			printf("No Weapon component found. Controller component failed to operate.\n");
			return;
		}
	}

	if (!m_pStamina) {
		if (pGO)
			m_pStamina = pGO->GetComponent<Stamina>(ComponentType::C_Stamina);
		else {
			printf("No Game Object found. Controller component failed to operate.\n");
			return;
		}

		if (!m_pWeapon) {
			printf("No Stamina component found. Controller component failed to operate.\n");
			return;
		}
	}

	TETRA_EVENTS.Subscribe(EVENT_INPUT_MOVE, this);
	TETRA_EVENTS.Subscribe(EVENT_INPUT_AIM, this);
	TETRA_EVENTS.Subscribe(EVENT_INPUT_FLY, this);
	TETRA_EVENTS.Subscribe(EVENT_INPUT_MELEE, this);
	TETRA_EVENTS.Subscribe(EVENT_INPUT_RANGE, this);
}