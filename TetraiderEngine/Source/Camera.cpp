#include <Stdafx.h>

Camera::Camera() :
	Component(ComponentType::C_Camera),
	m_fov(105.f), m_aspectRatio(1.f), m_screenWidth(1), m_screenHeight(1),
	m_viewMatrix(Matrix4x4()), m_cameraMatrix(Matrix4x4()), m_primary(false), m_isPersp(false), m_currentZoomIndex(0)
{
	std::fill_n(m_layersToRender, int(RENDER_LAYER::L_NUM_LAYERS), false);
}

Camera::~Camera(){}

void Camera::Deactivate() {
	pGO = nullptr;
	m_pTransform = nullptr; 
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

void Camera::Serialize(const json& j)
{
	m_primary = ParseBool(j, "primary");
	m_isPersp = ParseBool(j, "perspective");
	m_fov = ValueExists(j, "fov") ? ParseFloat(j, "fov") : m_fov;
	std::vector<std::string> layers = j["layers"];

	for (std::string layer : layers) {
		m_layersToRender[TETRA_GAME_OBJECTS.GetLayerFromString(layer)] = true;
	}

	for (unsigned int i = 0; i < MAX_RESOLUTIONS; ++i) {
		m_zoomLevels[i] = j["zoomLevels"][i];
	}
}

void Camera::HandleEvent(Event * pEvent)
{
	switch (pEvent->Type()) {
		case EventType::EVENT_WINDOW_RESIZED: {
			WindowResizedData* data = pEvent->Data<WindowResizedData>();
			m_screenWidth = data->resolution.width;
			m_screenHeight = data->resolution.height;
			m_currentZoomIndex = TETRA_GAME_CONFIG.GetCurrentResolutionIndex();
			break;
		}
	}
}

void Camera::LateInitialize()
{
	m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);

	if (m_primary)
		TETRA_GAME_OBJECTS.SetPrimaryCamera(pGO);

	TETRA_EVENTS.Subscribe(EventType::EVENT_WINDOW_RESIZED, this);
	
	Resolution res = TETRA_GAME_CONFIG.GetResolution();
	m_screenWidth = res.width;
	m_screenHeight = res.height;
	m_currentZoomIndex = TETRA_GAME_CONFIG.GetCurrentResolutionIndex();
}

void Camera::Update(float dt)
{
	//if (TETRA_INPUT.IsKeyPressed(SDL_SCANCODE_X))
	//	m_zoomLevels[m_currentZoomIndex] -= 0.01f;
	//	
	//if (TETRA_INPUT.IsKeyPressed(SDL_SCANCODE_Z))
	//	m_zoomLevels[m_currentZoomIndex] += 0.01f;
}

void Camera::LateUpdate(float dt)
{
	m_aspectRatio = TETRA_RENDERER.GetAspectRatio();
	_CalcViewMatrix();
	m_cameraMatrix = m_isPersp ? 
		Matrix4x4::Perspective(m_fov, m_aspectRatio, 1.f) : 
		Matrix4x4::Orthographic(float(m_screenWidth) * m_zoomLevels[m_currentZoomIndex], float(m_screenHeight) * m_zoomLevels[m_currentZoomIndex], 0.1f);
}

bool Camera::ShouldRenderLayer(RENDER_LAYER layer) const
{
	return m_layersToRender[layer];
}

bool Camera::ShouldRenderLayer(int layer) const
{
	return m_layersToRender[layer];
}

float Camera::GetFOV() const
{
	return m_fov;
}

float Camera::GetAspect() const
{
	return m_aspectRatio;
}

Vector3D Camera::TransformPointToScreenSpace(const Vector3D& worldCoordinates) {
	// TODO: talk to moodie
	Matrix4x4 viewPerspectiveMatrix = GetCameraMatrix() * GetViewMatrix(); 
	
	// Transform point to clipping coordinates
	Vector3D result = viewPerspectiveMatrix*worldCoordinates;
	result.x = ((result.x / result.w) + 1.f) / 2.0f * m_screenWidth*m_zoomLevels[m_currentZoomIndex];
	result.y = (1 - (result.y / result.w)) / 2.0f * m_screenHeight * m_zoomLevels[m_currentZoomIndex];
	result.z = 0;
	result.w = 1;

	return result;
}