#pragma once
#ifndef SNAP_TO_CAMERA_H
#define SNAP_TO_CAMERA_H

//#include "Component.h"
//class Transform;
//class GameObject;


/*
The intention of this component is for the GO it is attached to snap immediately to the
primary Camera's position.
*/
class SnapToCamera : public Component
{
private:
	Transform* m_pTransform;
	Transform* m_pCameraTransform;

public:
	SnapToCamera();
	~SnapToCamera();
	static Component* CreateInstance() { return new SnapToCamera(); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
};

#endif

