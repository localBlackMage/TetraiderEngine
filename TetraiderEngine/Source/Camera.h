#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "Math\Vector3D.h"
#include "Math\Matrix4x4.h"

class Transform;

class Camera : public Component
{
private:
	Transform* m_pTransform;

	float m_fov, m_aspectRatio;
	unsigned int m_screenWidth, m_screenHeight;
	Matrix4x4 m_viewMatrix, m_perspectiveMatrix, m_orthographicMatrix;

	Matrix4x4 _MatrixFromCameraVectors(const Vector3D& right, const Vector3D& up, const Vector3D& forward);
	void _CalcViewMatrix();
	void _CalcPerspectiveMatrix();
	void _CalcOrthographicMatrix();
public:
	Camera();
	~Camera();

	virtual void Serialize(json j);
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt) {};

	float GetFOV() const;
	float GetAspect() const;
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetPerspectiveMatrix() const;
	Matrix4x4 GetOrthographicMatrix() const;
};

#endif

