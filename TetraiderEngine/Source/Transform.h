#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
class Vector3D;
class Matrix4x4;

class Transform :public Component {
public:
	Transform();
	~Transform();
	Vector3D* m_pScale;
	Vector3D* m_pPivotOffset;
	Matrix4x4* m_pTransform;
	float m_angle;
	virtual void Update(float dt);
	virtual void Serialize(json j);
	virtual void LateInitialize() {}
	virtual void LateUpdate(float dt);
	const Vector3D* GetPosition();
	void SetPosition(const Vector3D* pPos);
private:
	Vector3D* m_pPosition;
};

#endif

