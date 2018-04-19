/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef FOLLOW_CAMERA_H
#define FOLLOW_CAMERA_H

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

