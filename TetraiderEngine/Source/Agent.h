#pragma once

#ifndef AGENT_H
#define AGENT_H

#include "Component.h"

enum class FaceDirection {
	Right,
	Left
};

// Forward declaration
class Transform;
class Body;
class Animation;

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
	FaceDirection GetFaceDirection() { return m_faceDirection; }
	void AddVelocity(const Vector3D& velocity) { m_currentVelocity += velocity; }
protected:
	float m_speed;
	float m_acceleration;
	float m_deceleration;
	float m_knockBackSpeed;
	Vector3D m_targetVelocity;
	Vector3D m_lookDirection;
	Transform* m_pTransform;
	Body* m_pBody;
	Animation* m_pAnimation;
	Vector3D GetDirectionToMouse();
	FaceDirection m_faceDirection;
private:
	Vector3D m_currentVelocity;
};

#endif

