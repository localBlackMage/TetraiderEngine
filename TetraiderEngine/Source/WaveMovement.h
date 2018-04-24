/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef WAVEMOVEMENT_H
#define WAVEMOVEMENT_H

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
	void SetActive(bool isActive) { m_isActive = isActive; }
	void SetInitialPos(const Vector3D& pos) { m_initialPos = pos; }
	bool IsActive() { return m_isActive; }
private:
	Transform* m_pTransform;
	bool m_isModifyXPos;
	bool m_isModifyYPos;
	bool m_isAbsoluteValue;
	bool m_isActive;
	float m_amplitude;
	float m_frequency;
	float m_phaseShift;
	Vector3D m_initialPos;
};

#endif

