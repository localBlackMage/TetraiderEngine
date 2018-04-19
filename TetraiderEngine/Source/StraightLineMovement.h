/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef STRAIGHT_LINE_MOVEMENT_H
#define STRAIGHT_LINE_MOVEMENT_H

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

