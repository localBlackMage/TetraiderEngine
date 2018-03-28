#include <Stdafx.h>

Camera::Camera() :
	Component(ComponentType::C_Camera),
	m_fov(105.f), m_aspectRatio(1.f), m_screenWidth(1), m_screenHeight(1),
	m_viewMatrix(Matrix4x4()), m_cameraMatrix(Matrix4x4()), m_primary(false), m_isPersp(false)
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
	m_zoom = ValueExists(j, "zoom") ? j["zoom"] : 1.f;
	m_isPersp = ParseBool(j, "perspective");
	m_fov = ValueExists(j, "fov") ? ParseFloat(j, "fov") : m_fov;
	std::vector<std::string> layers = j["layers"];

	for (std::string layer : layers) {
		m_layersToRender[TETRA_GAME_OBJECTS.GetLayerFromString(layer)] = true;
	}
}

void Camera::LateInitialize()
{
	m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);

	if (m_primary)
		TETRA_GAME_OBJECTS.SetPrimaryCamera(pGO);
}

void Camera::Update(float dt)
{
	// Test code
	if (m_primary) {
		if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_X))
			m_zoom -= 0.1f;
		
		if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_Z))
			m_zoom += 0.1f;
	}
}

void Camera::LateUpdate(float dt)
{
	m_aspectRatio = TETRA_RENDERER.GetAspectRatio();
	m_screenWidth = TETRA_RENDERER.WindowWidth();
	m_screenHeight = TETRA_RENDERER.WindowHeight();
	_CalcViewMatrix();
	m_cameraMatrix = m_isPersp ? 
		Matrix4x4::Perspective(m_fov, m_aspectRatio, 1.f) : 
		Matrix4x4::Orthographic(float(m_screenWidth) * m_zoom, float(m_screenHeight) * m_zoom, 0.1f);
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
	Matrix4x4 viewPerspectiveMatrix = Matrix4x4::Orthographic(float(m_screenWidth) * m_zoom, float(m_screenHeight) * m_zoom, 0.1f) * GetViewMatrix(); // TODO: talk to moodie
	// Transform point to clipping coordinates
	Vector3D result = viewPerspectiveMatrix*worldCoordinates;
	result.x = ((result.x / result.w) + 1.f) / 2.0f * m_screenWidth;
	result.y = (1 - (result.y / result.w)) / 2.0f * m_screenHeight;
	result.z = 0;
	result.w = 1;

	//result.x = (result.x + 1) / 2.0f*m_screenWidth;
	//result.y = (1 - result.y) / 2.0f*m_screenHeight;
	return result;
}