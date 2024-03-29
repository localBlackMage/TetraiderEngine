/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef SCREEN_WRAP_H
#define SCREEN_WRAP_H

class ScreenWrap : public Component
{
private:
	Transform* m_pTransform;
	Transform* m_pCameraTransform;
	Camera* m_pCamera;
	float m_xAllowance, m_yAllowance;	// How far off screen the object is allowed to go before it is reset and what position it is reset to on the opposite side
public:
	ScreenWrap();
	~ScreenWrap();
	static Component* CreateInstance() { return new ScreenWrap(); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
};

#endif

