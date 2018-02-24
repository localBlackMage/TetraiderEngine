#pragma once

#ifndef BODY_H
#define BODY_H

#include "Component.h"
#include "Math\MathLibs.h"

// Forward declaration
class Transform;
class Shape;

class Body : public Component {
public:
	bool m_isStatic;
	float m_mass;
	Shape* m_pShape;
	friend Transform;

	Body();
	~Body();
	static Component* CreateInstance() { return new Body(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt);

	void SetVelocity(const Vector3D& pVel);
	void SetVelocity(float speed, float angle);
	void Integrate(float dt);
	void AddForce(const Vector3D& force);
	const Vector3D& GetPosition() const;
	const Vector3D& GetPreviousPosition() const;
	const Vector3D& GetVelocity() const;
	void OverrideShapeData(float width, float height);
	void OverrideShapeData(float radius);
private:
	Transform* m_pTransform;
	Vector3D m_Acceleration;
	Vector3D m_Position;
	Vector3D m_PositionWithOffset;
	Vector3D m_Velocity;
	Vector3D m_PrevPosition;
	Vector3D m_Forces;
	Vector3D m_positionOffset;
	float m_massInv;
	void DrawDebugShape();
	void ClearForces();
};

#endif

