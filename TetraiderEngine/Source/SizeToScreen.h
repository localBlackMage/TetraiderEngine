#pragma once
#ifndef SIZE_TO_SCREEN_H
#define SIZE_TO_SCREEN_H

#include "Component.h"
class Transform;
class Camera;

/*
This component will set the pGO's Transform's Scale x & y to match that of the visible screen plus some offset.

Visible screen is defined as what's seen by the primary Camera (including zoom + screen dimensions).
*/
class SizeToScreen : public Component
{
private:
	Transform* m_pTransform;
	Camera* m_pCamera;
public:
	SizeToScreen();
	~SizeToScreen();
	static Component* CreateInstance() { return new SizeToScreen(); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
};

#endif

