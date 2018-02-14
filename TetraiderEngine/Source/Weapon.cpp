#include "Weapon.h"
#include "RangeAttack.h"
#include "MeleeAttack.h"
#include "Agent.h"
#include "GameObject.h"
#include "Controller.h"
#include "NPCController.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Animation.h"
#include "Audio.h"

Weapon::Weapon(): Component(C_Weapon) {}

Weapon::~Weapon() {
	for (auto attack : m_Attacks) {
		delete attack;
	}

	m_Attacks.clear();
	m_pWeapon->Destroy();
	m_pEffect->Destroy();
}

void Weapon::DeActivate() {
	for (auto attack : m_Attacks) {
		delete attack;
	}
	m_Attacks.clear();
	m_pWeapon->Destroy();
	m_pEffect->Destroy();
	m_pWeaponTransform = nullptr;
	m_pEffectTransform = nullptr;
	m_pController = nullptr;
}

void Weapon::Update(float dt) {
	for (auto attacks : m_Attacks) {
		attacks->Update(dt);
	}

	float angle = m_pController->GetLookDirection().AngleDegrees();
	if (m_pController->GetFaceDirection() == FaceDirection::Left) {
		m_pWeaponTransform->SetAngleZ(180 - angle*-1 - m_rotationOffset*swingDir);
		m_pEffectTransform->SetAngleZ(180 - angle*-1);
	}
	else {
		m_pWeaponTransform->SetAngleZ(angle - m_rotationOffset*swingDir);
		m_pEffectTransform->SetAngleZ(angle);
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
				AttackType::Ranged,
				ParseFloat(j["Attacks"][i], "projectileSpeed"),
				ParseFloat(j["Attacks"][i], "instantiationOffset"),
				ParseFloat(j["Attacks"][i], "lifeTime"),
				ParseString(j["Attacks"][i], "projectilePrefab")
			);

			attack->m_pOwner = this;
			m_Attacks.push_back(attack);
		}
	}

	m_weaponPrefab = ParseString(j, "weaponPrefab");
	m_pWeapon = TETRA_GAME_OBJECTS.CreateGameObject(m_weaponPrefab);
	m_weaponOffset.x = ParseFloat(j["weaponOffset"], "x");
	m_weaponOffset.y = ParseFloat(j["weaponOffset"], "y");
	m_weaponOffset.z = ParseFloat(j["weaponOffset"], "z");

	m_pWeaponTransform = m_pWeapon->GetComponent<Transform>(ComponentType::C_Transform);
	m_pWeaponTransform->SetPosition(m_weaponOffset);

	std::string m_effectPrefab = ParseString(j, "effectPrefab");
	m_pEffect = TETRA_GAME_OBJECTS.CreateGameObject(m_effectPrefab);
	if (m_pEffect) {
		m_pEffectTransform = m_pEffect->GetComponent<Transform>(ComponentType::C_Transform);
	}
}

void Weapon::LateInitialize() {
	m_pWeapon->SetParent(pGO);
	m_pEffect->SetParent(pGO);

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
}

void Weapon::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_FlipScaleX) {
		m_pWeapon->HandleEvent(pEvent);
		m_pEffect->HandleEvent(pEvent);
	}
}

void Weapon::PlayEffect() {
	Animation* pAnimation = m_pEffect->GetComponent<Animation>(ComponentType::C_Animation);
	if (pAnimation)
		pAnimation->Play(0, true);

	Swing();

	Audio* pAudio = m_pEffect->GetComponent<Audio>(ComponentType::C_Audio);
	if (pAudio)
		pAudio->Play();
}

// Assumes direction to be normalized
bool Weapon::UseAttack(int attack, const Vector3D& dirToAttack) {
	return m_Attacks[attack]->Use(dirToAttack);
}