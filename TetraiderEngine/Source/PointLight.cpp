#include "GameObject.h"
#include "PointLight.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Math/MathLibs.h"
#include <iostream>

PointLight::PointLight() :
	Component(ComponentType::C_PointLight)
{}

void PointLight::DeActivate() 
{ 
	pGO = nullptr; 
	m_pTransform = nullptr;
}

void PointLight::Serialize(const json& j) 
{
	m_color[0] = j["color"]["r"];
	m_color[1] = j["color"]["g"];
	m_color[2] = j["color"]["b"];
	m_color[3] = j["color"]["a"];

	m_distance = j["distance"];
}

void PointLight::Override(const json & j)
{
	if (ValueExists(j, "color")) {
		m_color[0] = ValueExists(j["color"], "r") ? j["color"]["r"] : m_color[0];
		m_color[1] = ValueExists(j["color"], "g") ? j["color"]["g"] : m_color[1];
		m_color[2] = ValueExists(j["color"], "b") ? j["color"]["b"] : m_color[2];
		m_color[3] = ValueExists(j["color"], "a") ? j["color"]["a"] : m_color[3];
	}

	m_distance = ValueExists(j, "distance") ? j["distance"] : m_distance;
}

Vector3D PointLight::GetPosition() const
{
	return m_pTransform->GetPosition();
}

void PointLight::LateInitialize()
{
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			std::cout << "No Game Object found. PointLight component failed to operate." << std::endl;
			return;
		}

		if (!m_pTransform) {
			std::cout << "No Transform component found. PointLight component failed to operate." << std::endl;
			assert(m_pTransform);
			return;
		}
	}
}
