#pragma once
#ifndef WAVEMOVEMENT_H
#define WAVEMOVEMENT_H

//#include "Component.h"
//#include "Math\MathLibs.h"
//
//class Transform;

class WaveMovement : public Component {
public:
	WaveMovement();
	~WaveMovement();
	static Component* CreateInstance() { return new WaveMovement(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	void ChangeInitialPos(const Vector3D& pos) { m_initialPos = pos; }
private:
	Transform* m_pTransform;
	bool m_isModifyXPos;
	bool m_isModifyYPos;
	bool m_isAbsoluteValue;
	float m_amplitude;
	float m_frequency;
	float m_phaseShift;
	Vector3D m_initialPos;
};

#endif

