/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

class Camera : public Component
{
private:
	Transform* m_pTransform;

	float m_fov, m_aspectRatio;
	unsigned int m_screenWidth, m_screenHeight;
	unsigned short m_currentZoomIndex;					// 
	Matrix4x4 m_viewMatrix, m_cameraMatrix;				// View matrix and perspective/orthographic matrix
	bool m_primary;										// True if this camera is the game's primary camera
	bool m_isPersp;										// True if this camera is Perspective, false if Orthographic
	bool m_layersToRender[RENDER_LAYER::L_NUM_LAYERS];	// True if this camera should render the respective layer, false if it should not
	float m_zoomLevels[MAX_RESOLUTIONS];								// Holds the zoom amount for each resolution loaded into GameConfig

	Matrix4x4 _MatrixFromCameraVectors(const Vector3D& right, const Vector3D& up, const Vector3D& forward);
	void _CalcViewMatrix();
public:
	Camera();
	~Camera();
	static Component* CreateInstance() { return new Camera(); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);

	Vector3D TransformPointToScreenSpace(const Vector3D& worldCoordinates);

	bool ShouldRenderLayer(RENDER_LAYER layer) const;
	bool ShouldRenderLayer(int layer) const;
	float GetFOV() const;
	float GetAspect() const;
	Matrix4x4 GetViewMatrix() const { return m_viewMatrix; };
	Matrix4x4 GetCameraMatrix() const { return m_cameraMatrix; };
	float ViewWidth() const { return m_screenWidth * m_zoomLevels[m_currentZoomIndex]; }
	float ViewHeight() const { return m_screenHeight * m_zoomLevels[m_currentZoomIndex]; }
	float GetZoom() { return m_zoomLevels[m_currentZoomIndex]; }
};

#endif

