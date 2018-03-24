#pragma once

#ifndef AGENT_H
#define AGENT_H

enum class FaceDirection {
	Right,
	Left
};

class Agent : public Component {
public:
	Agent(ComponentType _type);
	~Agent() {};
	virtual void Update(float dt);
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
	virtual void Deactivate();
	const Vector3D& GetCurrentVelocity() { return m_currentVelocity; }
	const Vector3D& GetTargetVelocity() { return m_targetVelocity; }
	const Vector3D& GetLookDirection() { return m_lookDirection; }
	Vector3D GetMovementDirection() { return Vector3D::Normalize(m_currentVelocity); }
	FaceDirection GetFaceDirection() { return m_faceDirection; }
	void AddVelocity(const Vector3D& velocity) { m_currentVelocity += velocity; }
	bool GetIgnoreHazard() { return m_isIgnoreHazards; }
	static Vector3D GetDirectionFromPlayerToMouse();
protected:
	float m_speed;
	float m_acceleration;
	float m_deceleration;
	float m_knockBackMultiplier;
	bool m_isDead;
	Vector3D m_targetVelocity;
	Vector3D m_lookDirection;
	Transform* m_pTransform;
	Body* m_pBody;
	Animation* m_pAnimation;
	FaceDirection m_faceDirection;
	bool m_isIgnoreHazards;
	bool m_isControlAnimationOnVelocity;
	Vector3D m_currentVelocity;

	void SetOrientation();
};

#endif

