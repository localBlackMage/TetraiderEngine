#include <Stdafx.h>


Controller::Controller() :
	Agent(ComponentType::C_Controller), m_isGameControllerEnabled(true), m_flySpeed(0), m_isControlsEnabled(true), m_flyOffset(1250.0f), m_isFlyingOutOfLevel(false), m_agility(1.0f), m_waitTimeBeforeFlyIn(0)
{
}

Controller::~Controller() {}

void Controller::Deactivate() {
	m_pWeapon = nullptr; 
	Agent::Deactivate(); 
}

void Controller::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	if (m_isFlyingInLevel) {
		if (m_waitTimeBeforeFlyIn < 0.5f) {
			m_waitTimeBeforeFlyIn += dt;
			return;
		}
		FlyIn();
	}
	else if (m_isFlyingOutOfLevel) {
		FlyOut();
	}

	Agent::Update(dt);
}

void Controller::LateUpdate(float dt) {}

void Controller::Serialize(const json& j) {
	Agent::Serialize(j["AgentData"]);
	m_flySpeed = ParseFloat(j, "flySpeed");
	m_agility = ParseFloat(j, "agility");
	m_pParticleEmitterGO = TETRA_GAME_OBJECTS.CreateGameObject(ParseString(j, "featherParticlePrefab"));
	m_pParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->DeactivateParticles();
}

void Controller::HandleEvent(Event* pEvent) {
	if (m_isDead) return;

	if (pEvent->Type() == EventType::EVENT_ShopOpened || pEvent->Type() == EventType::EVENT_OnEnterBoss) {
		m_targetVelocity = Vector3D();
		m_pBody->SetVelocity(Vector3D());
		m_isControlAnimationOnVelocity = true;
		m_isIgnoreHazards = false;
		m_isControlsEnabled = false;
		m_pParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->DeactivateParticles();
	}
	else if (pEvent->Type() == EventType::EVENT_ShopClosed || pEvent->Type() == EventType::EVENT_OnExitBoss)
		m_isControlsEnabled = true;

	if (!m_isControlsEnabled) return;

	if (pEvent->Type() == EventType::EVENT_OnLevelInitialized) {
		float agility;
		if (TETRA_PLAYERSTATS.IsPowerUpActive(PowerUpType::IncreaseAgility, agility))
			m_agility = agility;

		m_pWeapon->UpdateAttackSpeed(m_agility, 0);
		m_pWeapon->UpdateAttackSpeed(m_agility, 1);

		m_isFlyingInLevel = true;
		SetControlActive(false);
		pGO->m_isCollisionDisabled = true;
		m_posToFlyTo = m_pTransform->GetPosition();
		m_pTransform->SetPosition(m_pTransform->GetPosition() + Vector3D(-1, 0, 0) * m_flyOffset);
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_DisableCamFollow));
	}
	else if (pEvent->Type() == EventType::EVENT_ExitLevel) {
		m_isFlyingOutOfLevel = true;
		SetControlActive(false);
		pGO->m_isCollisionDisabled = true;
		m_posToFlyTo = m_pTransform->GetPosition() + Vector3D(1, 0, 0) * m_flyOffset;
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_DisableCamFollow));
	}

	if (TETRA_GAME_STATE.IsGamePaused()) {
		if (pEvent->Type() == EVENT_INPUT_FLY) {
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isPressed) m_isIgnoreHazards = true;
			else m_isIgnoreHazards = false;
		}
		return;
	}

	Agent::HandleEvent(pEvent);

	switch (pEvent->Type()) {
		case EVENT_INPUT_MOVE: {
			InputAxisData* pAxisData = pEvent->Data<InputAxisData>();
			if(!m_isIgnoreHazards) m_targetVelocity = pAxisData->m_dir*m_speed*m_agility;
			else {
				m_targetVelocity = pAxisData->m_dir*m_flySpeed*m_agility;
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
				m_isControlAnimationOnVelocity = false;
				m_pAnimation->Play(2);
				m_isIgnoreHazards = true;
				m_pParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->ActivateParticles();
			}
			else {
				m_isControlAnimationOnVelocity = true;
				m_isIgnoreHazards = false;
				m_pParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->DeactivateParticles();
			}
			break;
		}
		case EVENT_INPUT_MELEE: {
			if (m_isIgnoreHazards) return;
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isPressed)
				if (m_pWeapon->UseAttack(0, m_lookDirection)) 
					m_pWeapon->IsRotationOffset(true);
			break;
		}
		case EVENT_INPUT_RANGE: {
			if (m_isIgnoreHazards) return;
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isPressed) 
				if (m_pWeapon->UseAttack(1, m_lookDirection)) {
					m_pWeapon->IsRotationOffset(false);
					
				}
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
	TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
	TETRA_EVENTS.Subscribe(EVENT_ShopOpened, this);
	TETRA_EVENTS.Subscribe(EVENT_ShopClosed, this);
	TETRA_EVENTS.Subscribe(EVENT_OnEnterBoss, this);
	TETRA_EVENTS.Subscribe(EVENT_OnExitBoss, this);
}

void Controller::FlyIn() {
	m_isControlAnimationOnVelocity = false;
	m_pAnimation->Play(2);
	Vector3D dir(1, 0, 0);
	m_targetVelocity = dir*m_flySpeed*m_agility;
	m_lookDirection = dir;

	if (Vector3D::SquareDistance(m_pTransform->GetPosition(), m_posToFlyTo) < 500.0f) {
		m_isFlyingInLevel = false;
		m_isControlAnimationOnVelocity = true;
		SetControlActive(true);
		pGO->m_isCollisionDisabled = false;
		m_targetVelocity = Vector3D();
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_EnableCamFollow));
	}
}

void Controller::FlyOut() {
	m_isControlAnimationOnVelocity = false;
	m_pAnimation->Play(2);
	Vector3D dir(1, 0, 0);
	m_targetVelocity = dir*m_flySpeed*m_agility;
	m_lookDirection = dir;
}