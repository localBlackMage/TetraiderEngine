#pragma once

#ifndef AGENT_H
#define AGENT_H

#include "Component.h"

// Forward declaration
class Transform;
class Body;

class Agent : public Component {
public:
	Agent(ComponentType _type);
	~Agent() {};
	virtual void Update(float dt);
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
	const Vector3D& GetCurrentVelocity() { return m_currentVelocity; }
	const Vector3D& GetTargetVelocity() { return m_targetVelocity; }
	const Vector3D& GetLookDirection() { return m_lookDirection; }
	void AddVelocity(const Vector3D& velocity) { m_currentVelocity += velocity; }
protected:
	float m_speed;
	float m_acceleration;
	float m_deceleration;
	Vector3D m_targetVelocity;
	Vector3D m_lookDirection;
	Transform* m_pTransform;
	Body* m_pBody;
private:
	Vector3D m_currentVelocity;
};

#endif

