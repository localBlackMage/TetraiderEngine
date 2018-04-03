#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

//#include "Agent.h"

class Weapon;
//class Stamina;
//class ParticleEmitter;

class Controller : public Agent {
public:
	Controller();
	~Controller();
	static Component* CreateInstance() { return new Controller(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
	void SetControlActive(bool isActive) { m_isControlsEnabled = isActive; }
protected:
	Weapon* m_pWeapon;
	Stamina* m_pStamina;
	GameObject* m_pParticleEmitterGO;
private:
	bool m_isGameControllerEnabled;
	float m_flySpeed;
	float m_agility;
	float m_waitTimeBeforeFlyIn;
	bool m_isControlsEnabled;
	bool m_isFlyingInLevel;
	bool m_isFlyingOutOfLevel;

	void FlyIn();
	void FlyOut();
	Vector3D m_posToFlyTo;
	float m_flyOffset;
};

#endif

