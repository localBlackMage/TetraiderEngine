#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "Math\Vector3D.h"
#include "Math\Matrix4x4.h"

class Transform : public Component {
public:
	Transform();
	~Transform();
	Vector3D m_scale;
	Vector3D m_pivotOffset;
	Matrix4x4 m_transform;
	float m_angle;
	virtual void Update(float dt);
	virtual void Serialize(json j);
	virtual void LateInitialize() {}
	virtual void LateUpdate(float dt);
	const Vector3D GetPosition();
	void SetPosition(const Vector3D& pos);
private:
	Vector3D m_position;
};

#endif

