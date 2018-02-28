#pragma once
#ifndef STRAIGHT_LINE_MOVEMENT_H
#define STRAIGHT_LINE_MOVEMENT_H

#include "Component.h"
#include "Math\MathLibs.h"

class Transform;

class StraightLineMovement : public Component {
private:
	Transform* m_pTransform;

	float m_speed;
	Vector3D m_moveDirection;

public:
	StraightLineMovement();
	~StraightLineMovement();

	static Component* CreateInstance() { return new StraightLineMovement(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
};

#endif

