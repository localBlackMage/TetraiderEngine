#pragma once
#ifndef STAMINA_H
#define STAMINA_H

#include "Component.h"
#include "Math\Vector3D.h"

class Stamina : public Component {
public:
	Stamina();
	~Stamina();
	static Component* CreateInstance() { return new Stamina(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	bool IsStaminaEmpty() { return m_isEmpty; }
	bool IsStaminaFull() { return m_isFull; }
	bool UseStamina(float dt);
	float GetCurrentStamina() { return m_currentStamina; }
	float GetMaxStamina() { return m_maxStamina; }
private:
	float m_currentStamina;
	float m_maxStamina;
	float m_useSpeed;
	float m_rechargeSpeed;
	float m_rechargeCoolDown;
	float m_lastUsedTimeStamp;
	bool m_isEmpty;
	bool m_isFull;
};

#endif

