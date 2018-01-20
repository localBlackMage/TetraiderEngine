#include "Transform.h"

#include <iostream>

Transform::Transform() :
	Component(ComponentType::Transform),
	m_position(Vector3D()), 
	m_scale(Vector3D()), 
	m_transform(Matrix4x4()), 
	m_pivotOffset(Vector3D()),
	m_lookAt(Vector3D()),
	m_angleX(0.f), m_angleY(0.f), m_angleZ(0.f),
	m_is2d(true)
{
}

Transform::~Transform() 
{
}

void Transform::Update(float dt) {
	Matrix4x4 trans, rot, scal, pivotOffset;
	scal = Matrix4x4::Scale(m_scale.x, m_scale.y, m_scale.z);
	rot = Matrix4x4::Rotate(m_angleX, XAXIS) * Matrix4x4::Rotate(m_angleY, YAXIS) * Matrix4x4::Rotate(m_angleZ, ZAXIS);
	trans = Matrix4x4::Translate(m_position);
	pivotOffset = Matrix4x4::Translate(m_pivotOffset);

	m_transform = trans*rot*scal*pivotOffset;
}

void Transform::LateUpdate(float dt) {}

void Transform::Serialize(json j) {
	m_is2d = ValueExists(j, "transform", "2D") ? ParseBool(j, "transform", "2D") : true;
	m_position = ParseVector3D(j, "position");
	m_scale = ParseVector3D(j, "scale");
	m_angleX = ParseFloat(j, "rotation", "x");
	m_angleY = ParseFloat(j, "rotation", "y");
	m_angleZ = ParseFloat(j, "rotation", "z");
	m_pivotOffset = ParseVector3D(j, "pivotOffset");
}

#pragma region Translation
Vector3D Transform::GetPosition() const
{
	return m_position;// +(m_parentTransform ? m_parentTransform->GetPosition() : Vector3D());
}

void Transform::SetPosition(Vector3D pos)
{
	m_position = pos;

	//TODO: Update body position here
	/*
	Body* pBody = static_cast<Body*>(pGO->GetComponent(CT_BODY));
	if (pBody) {
	Vector3DSet(pBody->pPosition, pPosition);
	}*/
}

void Transform::Move(Vector3D amount)
{
	m_position += amount;
}
#pragma endregion

#pragma region Rotate
void Transform::_UpdateLookAt()
{
	m_lookAt = Matrix4x4::Rotate(GetAngleX(), Vector3D(1.0f, 0.0f, 0.0f, 0.0f)) *
		Matrix4x4::Rotate(GetAngleY(), Vector3D(0.0f, 1.0f, 0.0f, 0.0f)) *
		Matrix4x4::Rotate(GetAngleZ(), Vector3D(0.0f, 0.0f, 1.0f, 0.0f)) *
		Vector3D(0.0f, 1.0f, 0.0f, 0.0f);
}

void Transform::SetAngles(float angleX, float angleY, float angleZ)
{
	m_angleX = angleX;
	m_angleY = angleY;
	m_angleZ = angleZ;
	_UpdateLookAt();
}

float Transform::GetAngleX() const
{
	return m_angleX;// +(m_parentTransform ? m_parentTransform->GetAngleX() : .0f);
}

void Transform::SetAngleX(float angle)
{
	m_angleX = angle;
	_UpdateLookAt();
}

float Transform::GetAngleY() const
{
	return m_angleY;// +(m_parentTransform ? m_parentTransform->GetAngleY() : .0f);
}

void Transform::SetAngleY(float angle)
{
	m_angleY = angle;
	_UpdateLookAt();
}

float Transform::GetAngleZ() const
{
	return m_angleZ;// +(m_parentTransform ? m_parentTransform->GetAngleZ() : .0f);
}

void Transform::SetAngleZ(float angle)
{
	m_angleZ = angle;
	_UpdateLookAt();
}

void Transform::RotateX(float amount)
{
	m_angleX += amount;
	_UpdateLookAt();
}

void Transform::RotateY(float amount)
{
	m_angleY += amount;
	_UpdateLookAt();
}

void Transform::RotateZ(float amount)
{
	m_angleZ += amount;
	_UpdateLookAt();
}

Vector3D Transform::Forward() const
{
	return Vector3D::Normalize(m_lookAt);
}

Vector3D Transform::Right() const
{
	Vector3D up = m_is2d ? Vector3D(0, 0, 1, 0) : Vector3D(0, 1, 0, 0);
	Vector3D lCrossR = Vector3D::Cross(m_lookAt, up);
	float lenLCrossR = lCrossR.Length();
	return lenLCrossR != 0.0f ? lCrossR * (1.0f / lenLCrossR) : (m_is2d ? Vector3D(0, 1, 0, 0) : Vector3D(0, 0, 1, 0));
}

Vector3D Transform::Up() const
{
	return Vector3D::Cross(Forward() * -1, Right());
}

Vector3D Transform::LookAt() const
{
	return m_lookAt;
}
#pragma endregion

#pragma region Scale
float Transform::GetScaleX() const
{
	return m_scale.x;// * (m_parentTransform ? m_parentTransform->GetScaleX() : 1.f);
}

void Transform::SetScaleX(float scaleX)
{
	m_scale.x = scaleX;
}

void Transform::ScaleXby(float amount)
{
	m_scale.x += amount;
}

float Transform::GetScaleY() const
{
	return m_scale.y;// * (m_parentTransform ? m_parentTransform->GetScaleY() : 1.f);
}

void Transform::SetScaleY(float scaleY)
{
	m_scale.y = scaleY;
}

void Transform::ScaleYby(float amount)
{
	m_scale.y += amount;
}

float Transform::GetScaleZ() const
{
	return m_scale.z;// *(m_parentTransform ? m_parentTransform->GetScaleY() : 1.f);
}

void Transform::SetScaleZ(float scaleZ)
{
	m_scale.z = scaleZ;
}

void Transform::ScaleZby(float amount)
{
	m_scale.z += amount;
}

void Transform::SetScaleUniform(float amount)
{
	m_scale.Set(amount, amount, amount);
}

void Transform::SetScale(float scaleX, float scaleY)
{
	m_scale.Set(scaleX, scaleY, m_scale.z);
}

void Transform::SetScale(float scaleX, float scaleY, float scaleZ)
{
	m_scale.Set(scaleX, scaleY, scaleZ);
}

void Transform::ScaleUniform(float amount)
{
	m_scale += (Vector3D(1.f, 1.f, 1.f) * amount);
}
Matrix4x4 Transform::GetTransform() const
{
	return m_transform;
}
#pragma endregion
