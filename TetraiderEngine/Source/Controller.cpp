#include <Stdafx.h>


Controller::Controller() :
	Agent(ComponentType::C_Controller), 
	m_isGameControllerEnabled(true), 
	m_flySpeed(0), m_isControlsEnabled(true), 
	m_flyOffset(1250.0f), m_isFlyingOutOfLevel(false), 
	m_agility(1.0f), m_waitTimeBeforeFlyIn(0),
	m_flying(false),
	m_godModeDamageMultiplier(1),
	m_isGodMode(false)
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
	m_pFeatherParticleEmitterGO = TETRA_GAME_OBJECTS.CreateGameObject(ParseString(j, "featherParticlePrefab"));
	m_pFeatherParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->DeactivateParticles();

	m_featherPuffParticleEmitterPrefab = ParseString(j, "featherPuffParticlePrefab");
}

void Controller::HandleEvent(Event* pEvent) {
	if (m_isDead) return;

	switch (pEvent->Type()) {
		case EventType::EVENT_OnPlayerHealthZero: {
			m_isControlAnimationOnVelocity = false;
			m_isDead = true;
			m_pAnimation->Play(3);
			m_pWeapon->HideWeapon();
		}
		case EventType::EVENT_ShopOpened:
		case EventType::EVENT_OnEnterBoss: {
			m_targetVelocity = Vector3D();
			m_pBody->SetVelocity(Vector3D());
			m_isControlAnimationOnVelocity = true;
			m_isIgnoreHazards = false;
			m_isControlsEnabled = false;
			m_pFeatherParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->DeactivateParticles();
			break;
		}
		case EventType::EVENT_ShopClosed:
		case EventType::EVENT_OnExitBoss: {
			m_isControlsEnabled = true;
			break;
		}
		case EventType::EVENT_INPUT_GODMODE: {
			_ToggleGodMode();
			break;
		}
		case EventType::EVENT_OnCamGoToBossRoom: {
			Health* pHealth = pGO->GetComponent<Health>(C_Health);
			pHealth->Invincibility(true);
			m_targetVelocity = Vector3D();
			m_pBody->SetVelocity(Vector3D());
			m_isControlAnimationOnVelocity = true;
			m_isIgnoreHazards = false;
			m_isControlsEnabled = false;
			_StopFlying();
			break;
		}
		case EventType::EVENT_OnCamGoToPlayer: {
			Health* pHealth = pGO->GetComponent<Health>(C_Health);
			pHealth->Invincibility(false);
			m_isControlsEnabled = true;
			break;
		}
	}

	if (!m_isControlsEnabled) return;

	switch (pEvent->Type()) {
		case EventType::EVENT_OnLevelInitialized: {
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
			break;
		}
		case EventType::EVENT_ExitLevel: {
			Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
			if (pAudio)
				pAudio->Play();
			m_isFlyingOutOfLevel = true;
			SetControlActive(false);
			pGO->m_isCollisionDisabled = true;
			m_posToFlyTo = m_pTransform->GetPosition() + Vector3D(1, 0, 0) * m_flyOffset;
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_DisableCamFollow));
			break;
		}
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
			if(!m_isIgnoreHazards) 
				m_targetVelocity = pAxisData->m_dir*m_speed*m_agility;
			else {
				if (!pAxisData->m_dir.IsVectorZero()) {
					m_targetVelocity = pAxisData->m_dir*m_flySpeed*m_agility;
					m_lookDirection = pAxisData->m_dir;
				}
				else {
					m_targetVelocity = m_lookDirection*m_flySpeed*m_agility;
				}
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
			if (pButtonData->m_isPressed && (m_pStamina->UseStamina(TETRA_FRAMERATE.GetFrameTime()) || m_isGodMode)) {
				_Fly();
			}
			else {
				_StopFlying();
			}
			break;
		}
		case EVENT_INPUT_MELEE: {
			if (m_isIgnoreHazards) return;
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isPressed)
				if (m_pWeapon->UseAttack(0, m_lookDirection)) {
					m_pWeapon->SwapWeapons(false);
					m_pWeapon->IsRotationOffset(true);
				}
			break;
		}
		case EVENT_INPUT_RANGE: {
			if (m_isIgnoreHazards) return;
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isPressed) {
				if (m_pWeapon->UseAttack(1, m_lookDirection)) {
					m_pWeapon->SwapWeapons(true);
					m_pWeapon->IsRotationOffset(false);
				}
			}
			break;
		}
	}
}

void Controller::LateInitialize() {
	Agent::LateInitialize();

	if (m_pFeatherParticleEmitterGO) {
		m_pFeatherParticleEmitterGO->SetParent(pGO);
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
	TETRA_EVENTS.Subscribe(EVENT_INPUT_GODMODE, this);
	TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
	TETRA_EVENTS.Subscribe(EVENT_ShopOpened, this);
	TETRA_EVENTS.Subscribe(EVENT_ShopClosed, this);
	TETRA_EVENTS.Subscribe(EVENT_OnEnterBoss, this);
	TETRA_EVENTS.Subscribe(EVENT_OnExitBoss, this);
	TETRA_EVENTS.Subscribe(EVENT_OnPlayerHealthZero, this);
	TETRA_EVENTS.Subscribe(EVENT_OnCamGoToBossRoom, this);
	TETRA_EVENTS.Subscribe(EVENT_OnCamGoToPlayer, this);
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

void Controller::_Fly()
{
	if (!m_flying) {
		Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
		if (pAudio)
			pAudio->Play();
		GameObject* featherPuff = TETRA_GAME_OBJECTS.CreateGameObject(m_featherPuffParticleEmitterPrefab, true, m_pTransform->GetPosition());
		featherPuff->GetComponent<Transform>(C_Transform)->SetAngleZ(m_lookDirection.AngleDegrees() + 90.f);
		m_flying = true;
	}

	m_isControlAnimationOnVelocity = false;
	m_pAnimation->Play(2);
	m_isIgnoreHazards = true;
	m_pFeatherParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter)->ActivateParticles();
}

void Controller::_StopFlying()
{
	m_flying = false;
	m_isControlAnimationOnVelocity = true;
	m_isIgnoreHazards = false;
	ParticleEmitter * pParticleEmitter = m_pFeatherParticleEmitterGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter);
	pParticleEmitter->DeactivateParticles();
	pParticleEmitter->Reset();
}

void Controller::_ToggleGodMode() {
	m_isGodMode = !m_isGodMode;
	Health* pHealth = pGO->GetComponent<Health>(C_Health);
	pHealth->Invincibility(m_isGodMode);
	if (m_isGodMode) m_godModeDamageMultiplier = 2;
	else m_godModeDamageMultiplier = 1;
}
