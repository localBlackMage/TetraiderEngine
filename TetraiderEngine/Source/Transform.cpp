#include "Transform.h"

#include <iostream>

Transform::Transform() :
	Component(ComponentType::Transform),
	m_position(Vector3D()), 
	m_scale(Vector3D()), 
	m_transform(Matrix4x4()), 
	m_pivotOffset(Vector3D())
{
}

Transform::~Transform() 
{
}

void Transform::Update(float dt) {
	Matrix4x4 trans, rot, scal, pivotOffset;
	scal = Matrix4x4::Scale(m_scale.x, m_scale.y, m_scale.z);
	rot = Matrix4x4::Rotate(m_angle, ZAXIS);
	trans = Matrix4x4::Translate(m_position);
	pivotOffset = Matrix4x4::Translate(m_pivotOffset);

	m_transform = trans*rot*scal*pivotOffset;
}

void Transform::LateUpdate(float dt) {}

void Transform::Serialize(json j) {
	m_position = ParseVector3D(j, "position");
	m_scale = ParseVector3D(j, "scale");
	m_angle = ParseFloat(j, "rotation");
	m_pivotOffset = ParseVector3D(j, "pivotOffset");
}

const Vector3D Transform::GetPosition() {
	return m_position;
}

void Transform::SetPosition(const Vector3D& pos) {
	m_position = pos;

	//TODO: Update body position here
	/*
	Body* pBody = static_cast<Body*>(pGO->GetComponent(CT_BODY));
	if (pBody) {
		Vector3DSet(pBody->pPosition, pPosition);
	}*/
}