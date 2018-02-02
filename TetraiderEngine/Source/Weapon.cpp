#include "Weapon.h"
#include "RangeAttack.h"
#include "MeleeAttack.h"

Weapon::Weapon(): Component(C_Weapon) {}

Weapon::~Weapon() {
	for (auto attack : m_Attacks) {
		delete attack;
	}

	m_Attacks.clear();
}

void Weapon::Update(float dt) {
	for (auto attacks : m_Attacks)
		attacks->Update(dt);
}

void Weapon::Serialize(json j) {
	//TODO: This should not reallocate memory on level override
	int numberOfAttacks = j["Attacks"].size();

	//TODO: Move to a factory method if gets too complex
	for (int i = 0; i < numberOfAttacks; ++i) {
		if (ParseString(j["Attacks"][i], "type") == "Melee") {
			Attack* attack = new MeleeAttack(
				ParseFloat(j["Attacks"][i], "coolDown"),
				ParseInt(j["Attacks"][i], "baseDamage"),
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
				ParseFloat(j["Attacks"][i], "projectileSpeed"),
				ParseFloat(j["Attacks"][i], "instantiationOffset"),
				ParseFloat(j["Attacks"][i], "lifeTime"),
				ParseString(j["Attacks"][i], "projectilePrefab")
			);

			attack->m_pOwner = this;
			m_Attacks.push_back(attack);
		}
	}
}

void Weapon::LateInitialize() {}
void Weapon::HandleEvent(Event* pEvent) {}

// Assumes direction to be normalized
bool Weapon::UseAttack(int attack, const Vector3D& dirToAttack) {
	return m_Attacks[attack]->Use(dirToAttack);
}