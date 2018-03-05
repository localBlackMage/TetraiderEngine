#include "Transform.h"
#include "Body.h"
#include "GameObject.h"
#include <iostream>

#pragma region Private Methods

void Transform::_UpdateLookAt()
{
	m_lookAt = Matrix4x4::Rotate(GetAngleX(), XAXIS) *
		Matrix4x4::Rotate(GetAngleY(), YAXIS) *
		Matrix4x4::Rotate(GetAngleZ(), ZAXIS) *
		YAXIS;
}

void Transform::_UpdateBodyComponent()
{
	Body* pBody = pGO->GetComponent<Body>(ComponentType::C_Body);
	if (pBody)
		pBody->m_Position = m_position;
}

#pragma endregion

Transform::Transform() :
	Component(ComponentType::C_Transform),
	m_prevPosition(Vector3D()),
	m_position(Vector3D()), 
	m_scale(Vector3D()), 
	m_transform(Matrix4x4()), 
	m_pivotOffset(Vector3D()),
	m_lookAt(Vector3D(0.f, 1.f, 0.f)),
	m_angleX(0.f), m_angleY(0.f), m_angleZ(0.f),
	m_is2d(true)
{
}

Transform::~Transform() 
{
}

void Transform::Deactivate() {
	pGO = nullptr;
	m_parent = nullptr;
}

void Transform::Update(float dt) 
{
	m_prevPosition = m_position;
}

void Transform::LateUpdate(float dt) {
	// TODO: optimization if game object is static save m_Transform somewhere and never calculate matrix again
	Matrix4x4 trans;
	if (m_parent)
		trans = Matrix4x4::Translate(m_position + m_parent->GetPosition());
	else
		trans = Matrix4x4::Translate(m_position);
	
	Matrix4x4 scale(Matrix4x4::Scale(m_scale.x, m_scale.y, m_scale.z));
	Matrix4x4 rot(Matrix4x4::Rotate(m_angleZ, ZAXIS)); // Optimization, since 2D game only get Z axis. Revert if other axis are required
	//Matrix4x4 rot(Matrix4x4::Rotate(m_angleX, XAXIS) * Matrix4x4::Rotate(m_angleY, YAXIS) * Matrix4x4::Rotate(m_angleZ, ZAXIS));

	// TODO: Optimization, if pivot offset is zero do not create or multiply this component
	Matrix4x4 pivotOffset(Matrix4x4::Translate(m_pivotOffset));

	m_transform = trans*rot*scale*pivotOffset;
}

void Transform::Serialize(const json& j) {
	m_is2d = ValueExists(j, "2D") ? ParseBool(j, "2D") : true;
	m_angleX = ParseFloat(j, "rotation", "x");
	m_angleY = ParseFloat(j, "rotation", "y");
	m_angleZ = ParseFloat(j, "rotation", "z");

	m_position.x = ParseFloat(j["position"], "x");
	m_position.y = ParseFloat(j["position"], "y");
	m_position.z = ParseFloat(j["position"], "z");

	m_scale.x = ParseFloat(j["scale"], "x");
	m_scale.y = ParseFloat(j["scale"], "y");
	m_scale.z = ParseFloat(j["scale"], "z");

	m_pivotOffset.x = ParseFloat(j["pivotOffset"], "x");
	m_pivotOffset.y = ParseFloat(j["pivotOffset"], "y");
	m_pivotOffset.z = ParseFloat(j["pivotOffset"], "z");

	_UpdateLookAt();
}

void Transform::Override(const json & j)
{
	if (ValueExists(j, "position")) {
		m_position.x = ValueExists(j["position"], "x") ? j["position"]["x"] : m_position.x;
		m_position.y = ValueExists(j["position"], "y") ? j["position"]["y"] : m_position.y;
		m_position.z = ValueExists(j["position"], "z") ? j["position"]["z"] : m_position.z;
		_UpdateBodyComponent();
	}
	if (ValueExists(j, "scale")) {
		m_scale.x = ValueExists(j["scale"], "x") ? j["scale"]["x"] : m_scale.x;
		m_scale.y = ValueExists(j["scale"], "y") ? j["scale"]["y"] : m_scale.y;
		//m_scale.z = ValueExists(j["scale"], "z") ? j["scale"]["z"] : m_scale.z; // Likely not needed for our game
	}
	if (ValueExists(j, "rotation")) {
		m_angleZ = ValueExists(j["rotation"], "z") ? j["rotation"]["z"] : m_angleZ;
	}

	_UpdateLookAt();
}

void Transform::HandleEvent(Event * p_event) {
	switch (p_event->Type()) {
	case EventType::EVENT_FlipScaleX:
		if(m_parent) m_position.x *= -1;
		m_scale.x *= -1;
		break;
	case EventType::EVENT_FlipScaleY:
		if (m_parent) m_position.y *= -1;
		m_scale.y *= -1;
		break;
	}
}

bool Transform::operator<(const Transform & other) const
{
	return m_position.y > other.m_position.y;
}

#pragma region Translation
Vector3D Transform::GetPosition() const
{
	if (m_parent)
		return Vector3D(m_transform.Get(0, 3), m_transform.Get(1, 3), m_transform.Get(2, 3));
	else
		return m_position;
}

void Transform::SetPosition(const Vector3D& pos)
{
	m_position = pos;
	_UpdateBodyComponent();
}

void Transform::Move(const Vector3D& amount)
{
	m_position += amount;
	_UpdateBodyComponent();
}
#pragma endregion

#pragma region Rotate
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

Vector3D Transform::GetRotVector() const
{
	return Vector3D(m_angleX, m_angleY, m_angleZ);
}

Vector3D Transform::Forward() const
{
	return Vector3D::Normalize(m_lookAt);
}

Vector3D Transform::Right() const
{
	Vector3D up = m_is2d ? ZAXIS : YAXIS;
	Vector3D lCrossR = Vector3D::Cross(m_lookAt, up);
	float lenLCrossR = lCrossR.Length();
	return lenLCrossR != 0.0f ? lCrossR * (1.0f / lenLCrossR) : (m_is2d ? YAXIS : ZAXIS);
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
Vector3D Transform::GetScaleVector() const
{
	return m_scale;
}
Matrix4x4 Transform::GetTransform() const
{
	return m_transform;
}
Matrix4x4 Transform::GetTransformAfterOffset(const Vector3D & offset) const
{
	Matrix4x4 trans;
	if (m_parent)
		trans = Matrix4x4::Translate(m_position + offset + m_parent->GetPosition());
	else
		trans = Matrix4x4::Translate(m_position + offset);

	Matrix4x4 scale(Matrix4x4::Scale(m_scale.x, m_scale.y, m_scale.z));
	Matrix4x4 rot(Matrix4x4::Rotate(m_angleZ, ZAXIS)); // Optimization, since 2D game only get Z axis. Revert if other axis are required
													   //Matrix4x4 rot(Matrix4x4::Rotate(m_angleX, XAXIS) * Matrix4x4::Rotate(m_angleY, YAXIS) * Matrix4x4::Rotate(m_angleZ, ZAXIS));

													   // TODO: Optimization, if pivot offset is zero do not create or multiply this component
	Matrix4x4 pivotOffset(Matrix4x4::Translate(m_pivotOffset));

	return trans*rot*scale*pivotOffset;
}
Matrix4x4 Transform::TransformWithOffsetAndScale(const Vector3D & offset, const float & scaleX, const float & scaleY) const
{
	Matrix4x4 trans;
	if (m_parent)
		trans = Matrix4x4::Translate(m_position + offset + m_parent->GetPosition());
	else
		trans = Matrix4x4::Translate(m_position + offset);

	Matrix4x4 scale(Matrix4x4::Scale(scaleX, scaleY, 0));
	Matrix4x4 rot(Matrix4x4::Rotate(m_angleZ, ZAXIS)); // Optimization, since 2D game only get Z axis. Revert if other axis are required
													   //Matrix4x4 rot(Matrix4x4::Rotate(m_angleX, XAXIS) * Matrix4x4::Rotate(m_angleY, YAXIS) * Matrix4x4::Rotate(m_angleZ, ZAXIS));

													   // TODO: Optimization, if pivot offset is zero do not create or multiply this component
	Matrix4x4 pivotOffset(Matrix4x4::Translate(m_pivotOffset));

	return trans*rot*scale*pivotOffset;
}
#pragma endregion
