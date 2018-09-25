/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

HeightMap::HeightMap() : Component(ComponentType::C_HeightMap), m_maxHeight(0.0f) {}
HeightMap::~HeightMap() {}

void HeightMap::Update(float dt) {}

void HeightMap::LateUpdate(float dt) {}

void HeightMap::Deactivate() {
	pGO = nullptr;
}

void HeightMap::Serialize(const json& j) {
	m_maxHeight = ParseFloat(j, "height");
	std::string heightMapTexture = ParseString(j, "heightMap");
	if (heightMapTexture != "")
		m_pHeightMap = TETRA_RESOURCES.GetTexture(heightMapTexture);
}

void HeightMap::Override(const json& j) {}

void HeightMap::LateInitialize() {}

void HeightMap::HandleEvent(Event* pEvent) {
}
