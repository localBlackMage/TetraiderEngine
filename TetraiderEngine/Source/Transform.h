#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "Math\Vector3D.h"
#include "Math\Matrix4x4.h"

class Transform : public Component {
private:
	Vector3D m_position;
	Vector3D m_scale;
	Vector3D m_pivotOffset;
	Matrix4x4 m_transform;
	Vector3D m_lookAt;
	float m_angleX, m_angleY, m_angleZ;
	bool m_is2d;

	void _UpdateLookAt();
public:
	Transform();
	~Transform();

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(json j);
	virtual void Override(json j) {};

	void SetPosition(Vector3D pos);
	Vector3D GetPosition() const;
	void Move(Vector3D amount);

	void SetAngles(float angleX, float angleY, float angleZ);
	float GetAngleX() const;
	void SetAngleX(float angle);
	float GetAngleY() const;
	void SetAngleY(float angle);
	float GetAngleZ() const;
	void SetAngleZ(float angle);

	void RotateX(float amount);
	void RotateY(float amount);
	void RotateZ(float amount);

	Vector3D Forward() const;
	Vector3D Right() const;
	Vector3D Up() const;

	Vector3D LookAt() const;

	float GetScaleX() const;
	void SetScaleX(float scaleX);
	void ScaleXby(float amount);

	float GetScaleY() const;
	void SetScaleY(float scaleY);
	void ScaleYby(float amount);

	float GetScaleZ() const;
	void SetScaleZ(float scaleZ);
	void ScaleZby(float amount);

	void SetScaleUniform(float amount);
	void SetScale(float scaleX, float scaleY);
	void SetScale(float scaleX, float scaleY, float scaleZ);
	void ScaleUniform(float amount);

	Matrix4x4 GetTransform() const;
};

#endif

