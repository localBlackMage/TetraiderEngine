#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "Component.h"

class Attack;
class Transform;
class Agent;

class Weapon : public Component {
public:
	Weapon();
	~Weapon();
	static unsigned int CreateInstance() { return static_cast<unsigned int>(C_Weapon); }
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt);
	virtual void HandleEvent(Event* pEvent);
	void PlayEffect();
	bool UseAttack(int attack, const Vector3D& dirToAttack);
	void Swing() { swingDir *= -1; }
private:
	std::vector<Attack*> m_Attacks;
	std::string m_weaponPrefab;
	GameObject* m_pWeapon;
	Transform* m_pWeaponTransform;
	std::string m_effectPrefab;
	GameObject* m_pEffect;
	Transform* m_pEffectTransform;
	Agent* m_pController;
	Vector3D m_weaponOffset;
	int swingDir = 1;
	float m_rotationOffset;
};

#endif
