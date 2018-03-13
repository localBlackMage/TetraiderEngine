#include <Stdafx.h>

LightBase::LightBase(ComponentType cType) :
	Component(cType)
{
	memset(m_layers, false, RENDER_LAYER::L_NUM_LAYERS);
}

void LightBase::DeActivate()
{
	pGO = nullptr;
	m_pTransform = nullptr;
}

void LightBase::Serialize(const json& j)
{
	Vector3D color = ParseColor(j, "color");
	m_color.r = int(color.x * 255.f);
	m_color.g = int(color.y * 255.f);
	m_color.b = int(color.z * 255.f);
	m_color.a = int(color.w * 255.f);

	m_offset = ParseVector3D(j, "offset");

	m_distance = j["distance"];

	std::vector< std::string > layers = ParseStringList(j, "layers");

	for (std::string layer : layers) {
		int idx = TETRA_GAME_OBJECTS.GetLayerFromString(layer);
		if (idx < RENDER_LAYER::L_NUM_LAYERS)
			m_layers[idx] = true;
	}
}

void LightBase::Override(const json & j)
{
	if (ValueExists(j, "color")) {
		Vector3D color = ParseColor(j, "color");
		m_color.r = int(color.x * 255.f);
		m_color.g = int(color.y * 255.f);
		m_color.b = int(color.z * 255.f);
		m_color.a = int(color.w * 255.f);
	}

	m_distance = ValueExists(j, "distance") ? j["distance"] : m_distance;
}

Vector3D LightBase::GetPosition() const
{
	return m_pTransform->GetPosition() + m_offset;
}

void LightBase::LateInitialize()
{
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			std::cout << "No Game Object found. LightBase component failed to operate." << std::endl;
			return;
		}

		if (!m_pTransform) {
			std::cout << "No Transform component found. LightBase component failed to operate." << std::endl;
			assert(m_pTransform);
			return;
		}
	}
}
