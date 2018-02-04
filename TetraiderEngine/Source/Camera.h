#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "Layers.h"
#include "Math\MathLibs.h"

class Transform;
enum RENDER_LAYER;

class Camera : public Component
{
private:
	Transform* m_pTransform;

	float m_fov, m_aspectRatio;
	unsigned int m_screenWidth, m_screenHeight;
	Matrix4x4 m_viewMatrix, m_cameraMatrix;
	bool m_isPersp;
	bool m_layersToRender[RENDER_LAYER::L_NUM_LAYERS];

	Matrix4x4 _MatrixFromCameraVectors(const Vector3D& right, const Vector3D& up, const Vector3D& forward);
	void _CalcViewMatrix();
public:
	Camera();
	~Camera();

	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(json j);

	Vector3D TransformPointToScreenSpace(const Vector3D& worldCoordinates);

	bool ShouldRenderLayer(RENDER_LAYER layer) const;
	bool ShouldRenderLayer(int layer) const;
	float GetFOV() const;
	float GetAspect() const;
	Matrix4x4 GetViewMatrix() const { return m_viewMatrix; };
	Matrix4x4 GetCameraMatrix() const { return m_cameraMatrix; };
};

#endif

