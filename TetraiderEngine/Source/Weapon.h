#pragma once
#ifndef WEAPON_H
#define WEAPON_H

class Weapon : public Component {
private:
	std::vector<Attack*> m_Attacks;
	std::string m_weaponPrefab;
	std::string m_secondaryWeaponPrefab;
	std::string m_fireArrowPrefab;
	GameObject* m_pWeapon;
	GameObject* m_pSecondaryWeapon;
	Transform* m_pWeaponTransform;
	std::string m_effectPrefab;
	GameObject* m_pEffect;
	Transform* m_pEffectTransform;
	Agent* m_pController;
	Vector3D m_weaponOffset;
	int swingDir = 1;
	float m_rotationOffset;
	bool m_isRotationOffset;
	int m_damageMultiplier;
	bool m_isSecondaryCurrentlyEquipped;
	void CheckForPowerUps();

public:
	Weapon();
	~Weapon();

	static Component* CreateInstance() { return new Weapon(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt);
	virtual void HandleEvent(Event* pEvent);

	float GetCoolDown(int attack) { return m_Attacks[attack]->GetCoolDown(); }
	void PlayEffect();
	bool UseAttack(int attack, const Vector3D& dirToAttack);
	bool UseAttack(int attack, const Vector3D& dirToAttack, int &ammo);

	int GetAmmo(int attack) const;
	void AddAmmo(int attack, int value);
	void SetAmmo(int attack, int value);

	void UpdateAttackSpeed(float multiplier, int attack);
	void IncreaseMeleeAttackRange(float multiplier, int attack);
	void Swing() { swingDir *= -1; }
	void IsRotationOffset(bool active) { m_isRotationOffset = active; }
	void MultiplyDamage(float multiplier, int attack);
	void HideWeapon(bool active = true);
	void SwapWeapons(bool sisScondary);
	void ModifyProjectileSpeed(float speed, int attack = 0);
	int GetDamageMultiplier();
	inline float GetRotationOffset() const { return m_rotationOffset; }
};

#endif
