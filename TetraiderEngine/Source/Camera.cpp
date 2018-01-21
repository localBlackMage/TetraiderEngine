#include "Camera.h"
#include "Transform.h"
#include "RenderManager.h"

Camera::Camera() :
	Component(ComponentType::Camera),
	m_fov(105.f), m_aspectRatio(1.f), m_screenWidth(1), m_screenHeight(1),
	m_viewMatrix(Matrix4x4()), m_perspectiveMatrix(Matrix4x4()), m_orthographicMatrix(Matrix4x4())
{

}

Camera::~Camera()
{
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
	//Matrix4x4 rotationM = _MatrixFromCameraVectors(m_pTransform->Right(), m_pTransform->Up(), m_pTransform->Forward());
	Matrix4x4 rotationM = _MatrixFromCameraVectors(XAXIS, YAXIS, ZAXIS * -1);
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

void Camera::Serialize(json j)
{
	m_fov = ValueExists(j, "fov") ? ParseFloat(j, "fov") : m_fov;
}

void Camera::Override(json j)
{
	m_fov = ValueExists(j, "fov") ? ParseFloat(j, "fov") : m_fov;
}

void Camera::LateInitialize()
{
	m_pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
}

void Camera::Update(float dt)
{

}

void Camera::LateUpdate(float dt)
{
	RenderManager& renderMngr = RenderManager::GetInstance();
	m_aspectRatio = renderMngr.GetAspectRatio();
	m_screenWidth = renderMngr.WindowWidth();
	m_screenHeight = renderMngr.WindowHeight();
	_CalcViewMatrix();
	//switch (m_cameraType) {
	//case CAM_BOTH:
	//	_CalcPerspectiveMatrix();
	//	_CalcOrthographicMatrix();
	//	break;
	//case CAM_PERSP:
	//	_CalcPerspectiveMatrix();
	//	break;
	//case CAM_ORTHO:
	_CalcOrthographicMatrix();
	//	break;
	//}
}

float Camera::GetFOV() const
{
	return m_fov;
}

float Camera::GetAspect() const
{
	return m_aspectRatio;
}

Matrix4x4 Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

Matrix4x4 Camera::GetPerspectiveMatrix() const
{
	return m_perspectiveMatrix;
}

Matrix4x4 Camera::GetOrthographicMatrix() const
{
	return m_orthographicMatrix;
}