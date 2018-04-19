/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef DESTROY_ONHEALTH_ZERO_H
#define DESTROY_ONHEALTH_ZERO_H

class DestroyOnHealthZero : public Component {
public:
	DestroyOnHealthZero();
	~DestroyOnHealthZero();
	static Component* CreateInstance() { return new DestroyOnHealthZero(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	float m_destroyIn;
	float m_timeStamp;
	bool m_isDestory;
	bool m_isExplode;
	bool m_isSpawnAcidPool;
	float m_explosionRadius;
	float m_knockBackSpeed;
	int m_explosionDamage;
	std::string m_explosionPrefab;
	std::string m_acidPrefab;
	Attack* m_Attack;
};

#endif

