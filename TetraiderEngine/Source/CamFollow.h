#pragma once
#ifndef CAM_FOLLOW_H
#define CAM_FOLLOW_H

//#include "Component.h"
//class Transform;
//class GameObject;

class CamFollow : public Component
{
private:
	Transform* m_pTransform;
	GameObject* m_pTarget;
	Vector3D bossTarget;
	float m_followSpeed;
	float m_showSpeed;
	float m_showTime;
	std::string m_defaultTargetTag;
	bool m_isActive;
	bool m_isGoToBossRoom;
public:
	CamFollow();
	~CamFollow();
	static Component* CreateInstance() { return new CamFollow(); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
};

#endif

