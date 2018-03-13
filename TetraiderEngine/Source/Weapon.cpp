#include "Stdafx.h"

Weapon::Weapon(): Component(C_Weapon), m_pEffect(nullptr), m_isRotationOffset(true) {}

Weapon::~Weapon() {
	for (auto attack : m_Attacks) {
		delete attack;
	}

	m_Attacks.clear();
	if(m_pWeapon) m_pWeapon->Destroy();
	if(m_pEffect) m_pEffect->Destroy();
}

void Weapon::Deactivate() {
	for (auto attack : m_Attacks) {
		delete attack;
	}

	m_Attacks.clear();
	if (m_pWeapon) m_pWeapon->Destroy();
	if (m_pEffect) m_pEffect->Destroy();
	m_pWeaponTransform = nullptr;
	m_pEffectTransform = nullptr;
	m_pEffect = nullptr;
	m_pController = nullptr;
}

void Weapon::Update(float dt) {
	for (auto attacks : m_Attacks) {
		attacks->Update(dt);
	}

	float angle = m_pController->GetLookDirection().AngleDegrees();
	if (m_pController->GetFaceDirection() == FaceDirection::Left) {
		if (m_pWeapon) {
			if (m_isRotationOffset)
				m_pWeaponTransform->SetAngleZ(180 - angle*-1 - m_rotationOffset*swingDir);
			else
				m_pWeaponTransform->SetAngleZ(180 - angle*-1);
		}
		if(m_pEffect) m_pEffectTransform->SetAngleZ(180 - angle*-1);
	}
	else {
		if (m_pWeapon) {
			if (m_isRotationOffset)
				m_pWeaponTransform->SetAngleZ(angle - m_rotationOffset*swingDir);
			else
				m_pWeaponTransform->SetAngleZ(angle);
		}
		if (m_pEffect) m_pEffectTransform->SetAngleZ(angle);
	}
}

void Weapon::LateUpdate(float dt) {
	for (auto attacks : m_Attacks) {
		// Debug
		if (attacks->GetType() == AttackType::Melee) {
			if (m_pController)
				attacks->Debug(m_pController->GetLookDirection());
		}
	}
}

void Weapon::Serialize(const json& j) {
	//TODO: Cannot override weapon component at the moment
	if (m_Attacks.size() > 0) 
		return;

	int numberOfAttacks = j["Attacks"].size();
	m_rotationOffset = ParseFloat(j, "rotationOffset");

	//TODO: Move to a factory method if gets too complex
	for (int i = 0; i < numberOfAttacks; ++i) {
		if (ParseString(j["Attacks"][i], "type") == "Melee") {
			Attack* attack = new MeleeAttack(
				ParseFloat(j["Attacks"][i], "coolDown"),
				ParseInt(j["Attacks"][i], "baseDamage"),
				ParseFloat(j["Attacks"][i], "knockBackSpeed"),
				AttackType::Melee,
				ParseFloat(j["Attacks"][i], "radius"),
				ParseFloat(j["Attacks"][i], "angle"),
				ParseFloat(j["Attacks"][i], "triggerAttackIn")
			);

			attack->m_pOwner = this;
			m_Attacks.push_back(attack);
		}
		else if (ParseString(j["Attacks"][i], "type") == "Ranged") {
			Attack* attack = new RangeAttack(
				ParseFloat(j["Attacks"][i], "coolDown"),
				ParseInt(j["Attacks"][i], "baseDamage"),
				ParseFloat(j["Attacks"][i], "knockBackSpeed"),
				ParseInt(j["Attacks"][i], "ammo"),
				ParseBool(j["Attacks"][i], "isUnlimitedAmmo"),
				AttackType::Ranged,
				ParseFloat(j["Attacks"][i], "projectileSpeed"),
				ParseFloat(j["Attacks"][i], "instantiationOffset"),
				ParseFloat(j["Attacks"][i], "lifeTime"),
				ParseInt(j["Attacks"][i], "multipleShots"),
				ParseFloat(j["Attacks"][i], "coneAngle"),
				ParseString(j["Attacks"][i], "projectilePrefab")
			);

			attack->m_pOwner = this;
			m_Attacks.push_back(attack);
		}
	}

	if (!TETRA_GAME_STATE.IsEditorMode()) {
		m_weaponPrefab = ParseString(j, "weaponPrefab");
		if (m_weaponPrefab != "") {
			m_pWeapon = TETRA_GAME_OBJECTS.CreateGameObject(m_weaponPrefab);
			m_weaponOffset.x = ParseFloat(j["weaponOffset"], "x");
			m_weaponOffset.y = ParseFloat(j["weaponOffset"], "y");
			m_weaponOffset.z = ParseFloat(j["weaponOffset"], "z");

			m_pWeaponTransform = m_pWeapon->GetComponent<Transform>(ComponentType::C_Transform);
			m_pWeaponTransform->SetPosition(m_weaponOffset);
		}

		if (ValueExists(j, "effectPrefab")) {
			std::string m_effectPrefab = ParseString(j, "effectPrefab");
			m_pEffect = TETRA_GAME_OBJECTS.CreateGameObject(m_effectPrefab);
			if (m_pEffect) {
				m_pEffectTransform = m_pEffect->GetComponent<Transform>(ComponentType::C_Transform);
			}
		}
	}
}

void Weapon::LateInitialize() {
	if(m_pWeapon) m_pWeapon->SetParent(pGO);
	if(m_pEffect) m_pEffect->SetParent(pGO);

	if (!m_pController) {
		m_pController = pGO->GetComponent<Controller>(ComponentType::C_Controller);
		if (!m_pController) {
			m_pController = pGO->GetComponent<NPCController>(ComponentType::C_NPCCONTROLLER);
			if (!m_pController) {
				printf("No Controller component found. Weapon component failed to operate.\n");
				assert(m_pController);
				return;
			}
		}
	}

	if (pGO->m_tag == T_Player) {
		TETRA_EVENTS.Subscribe(EventType::EVENT_AmmoUpdate, this);
	}
}

void Weapon::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_FlipScaleX) {
		if(m_pWeapon) m_pWeapon->HandleEvent(pEvent);
		if(m_pEffect) m_pEffect->HandleEvent(pEvent);
	}
	else if (pEvent->Type() == EVENT_OnLevelInitialized) {
		if (pGO->m_tag == T_Player) {
			int ammo = GetAmmo(1);
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_UIAmmoUpdate, &CollectibleData(ammo)));
			CheckForPowerUps();
		}
	}
	else if (pEvent->Type() == EVENT_AmmoUpdate) {
		CollectibleData* pData = pEvent->Data<CollectibleData>();
		if (pData) {
			AddAmmo(1, pData->m_value);
			pData->m_value = GetAmmo(1);
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_UIAmmoUpdate, pData));
		}
	}
}

void Weapon::PlayEffect() {
	if (m_pEffect) {
		Animation* pAnimation = m_pEffect->GetComponent<Animation>(ComponentType::C_Animation);
		if (pAnimation)
			pAnimation->Play(0, true);

		Swing();

		if (pGO->m_tag == T_Player) {
			Audio* pAudio = m_pEffect->GetComponent<Audio>(ComponentType::C_Audio);
			if (pAudio)
				pAudio->Play();
		}
	}
}

// Assumes direction to be normalized
bool Weapon::UseAttack(int attack, const Vector3D& dirToAttack) {
	return m_Attacks[attack]->Use(dirToAttack);
}

bool Weapon::UseAttack(int attack, const Vector3D& dirToAttack, int& ammo) {
	return m_Attacks[attack]->Use(dirToAttack);
}

int Weapon::GetAmmo(int attack) const {
	RangeAttack* pAttack = static_cast<RangeAttack*>(m_Attacks[attack]);
	if (pAttack)
		return pAttack->GetAmmo();
	else
		return 0;
}

void Weapon::AddAmmo(int attack, int ammo) {
	RangeAttack* pAttack = static_cast<RangeAttack*>(m_Attacks[attack]);
	if (pAttack)
		return pAttack->UpdateAmmo(ammo + pAttack->GetAmmo());
}

void Weapon::SetAmmo(int attack, int ammo) {
	RangeAttack* pAttack = static_cast<RangeAttack*>(m_Attacks[attack]);
	if (pAttack) {
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_UIAmmoUpdate, &CollectibleData(ammo)));
		return pAttack->UpdateAmmo(ammo);
	}
}

void Weapon::CheckForPowerUps() {
	if (TETRA_PLAYERSTATS.IsTripleShotActive()) {
		// Hardcoded for now
		m_Attacks[1]->AddDamage(-2);
		RangeAttack* pAttack = static_cast<RangeAttack*>(m_Attacks[1]);
		pAttack->SetMultipleShot(3, 4);
	}
}