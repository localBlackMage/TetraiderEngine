#ifndef AGENT_H
#define AGENT_H

#include "Component.h"
#include "..\Source\Math\Vector3D.h"

// Forward declaration
class Transform;
class Body;

class Agent : public Component {
public:
	Agent(ComponentType _type) : Component(_type), m_speed(0.0f), m_acceleration(0.0f), m_deceleration(0.0f) {};
	~Agent() {};
	virtual void Update(float dt);
	virtual void LateInitialize();
	virtual void Serialize(json j);
	virtual void HandleEvent(Event* pEvent);
	const Vector3D& GetCurrentVelocity() { return m_currentVelocity; }
	const Vector3D& GetTargetVelocity() { return m_targetVelocity; }
	void AddVelocity(const Vector3D& velocity) { m_currentVelocity += velocity; }
protected:
	float m_speed;
	float m_acceleration;
	float m_deceleration;
	Vector3D m_targetVelocity;
	Transform* m_pTransform;
	Body* m_pBody;
private:
	Vector3D m_currentVelocity;
};

#endif

