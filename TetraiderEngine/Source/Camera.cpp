#include "Camera.h"
#include "Transform.h"

Camera::Camera() :
	Component(ComponentType::Camera),
	m_fov(105.f), m_aspectRatio(1.f), m_screenWidth(1), m_screenHeight(1),
	m_viewMatrix(Matrix4x4()), m_perspectiveMatrix(Matrix4x4()), m_orthographicMatrix(Matrix4x4())
{

}

Camera::~Camera()
{
}

void Camera::LateInitialize()
{
	m_pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
}

Matrix4x4 Camera::_MatrixFromCameraVectors(const Vector3D & right, const Vector3D & up, const Vector3D & forward)
{
	return Matrix4x4(
		right.x, right.y, right.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		-forward.x, -forward.y, -forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void Camera::_CalcViewMatrix()
{
	Matrix4x4 rotationM = _MatrixFromCameraVectors(m_pTransform->Right(), m_pTransform->Up(), m_pTransform->Forward());
	m_viewMatrix = rotationM * Matrix4x4::Translate(-1 * m_pTransform->GetPosition());
}

void Camera::_CalcPerspectiveMatrix()
{
	m_perspectiveMatrix = Matrix4x4::Perspective(m_fov, m_aspectRatio, 1.f);
}

void Camera::_CalcOrthographicMatrix()
{
	m_orthographicMatrix = Matrix4x4::Orthographic(m_screenWidth, m_screenHeight, 0.1f);
}
