#include <Stdafx.h>

PointLight::PointLight() :
	LightBase(ComponentType::C_PointLight)
{}

void PointLight::SetActive(bool active) {
	if (!active) {
		m_distance = 0;
	}
	else {
		m_distance = 1000;
	}
}
