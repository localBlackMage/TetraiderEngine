#include "Transform.h"
#include "../Source/Math/Vector3D.h"
#include "../Source/Math/Matrix4x4.h"

#include <iostream>

const Vector3D ZAXIS = Vector3D(0, 0, 1, 0);

Transform::Transform() {
	type = ComponentType::Transform;

	m_pPosition = new Vector3D();
	m_pScale = new Vector3D();
	m_pTransform = new Matrix4x4();
	m_pPivotOffset = new Vector3D();
}

Transform::~Transform() {
	delete m_pPosition;
	delete m_pScale;
	delete m_pTransform;
	delete m_pPivotOffset;
}

void Transform::Update(float dt) {
	Matrix4x4 trans, rot, scal, pivotOffset;
	scal = Matrix4x4::Scale(m_pScale->x, m_pScale->y, m_pScale->z);
	rot = Matrix4x4::Rotate(m_angle, ZAXIS);
	trans = Matrix4x4::Translate(*m_pPosition);
	pivotOffset = Matrix4x4::Translate(*m_pPivotOffset);

	*m_pTransform = trans*rot*scal*pivotOffset;
}

void Transform::LateUpdate(float dt) {}

void Transform::Serialize(json j) {
	m_pPosition->x = ParseFloat(j, "positionX");
	m_pPosition->y = ParseFloat(j, "positionY");
	m_pPosition->z = ParseFloat(j, "positionZ");
	m_pScale->x = ParseFloat(j, "scaleX");
	m_pScale->y = ParseFloat(j, "scaleY");
	m_pScale->z = ParseFloat(j, "scaleZ");
	m_angle = ParseFloat(j, "rotation");
	m_pPivotOffset->x = ParseFloat(j, "pivotOffsetX");
	m_pPivotOffset->y = ParseFloat(j, "pivotOffsetY");
}

const Vector3D* Transform::GetPosition() {
	return m_pPosition;
}

void Transform::SetPosition(const Vector3D *pPos) {
	m_pPosition->Set(pPos->x, pPos->y, pPos->z);

	//TODO: Update body position here
	/*
	Body* pBody = static_cast<Body*>(pGO->GetComponent(CT_BODY));
	if (pBody) {
		Vector3DSet(pBody->pPosition, pPosition);
	}*/
}