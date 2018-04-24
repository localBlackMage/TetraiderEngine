/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

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
