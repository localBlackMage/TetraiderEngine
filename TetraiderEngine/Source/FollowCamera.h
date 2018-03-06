#pragma once
#ifndef FOLLOW_CAMERA_H
#define FOLLOW_CAMERA_H

//#include "Component.h"
//class Transform;
//class GameObject;


/*
The intention of this component is for the GO it is attached to to follow the
primary Camera. Any movement made by the primary Camera will be reflected in this
component's parent GO.
*/
class FollowCamera : public Component
{
private:
	Transform* m_pTransform;
	Transform* m_pCameraTransform;

public:
	FollowCamera();
	~FollowCamera();
	static Component* CreateInstance() { return new FollowCamera(); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
};

#endif

