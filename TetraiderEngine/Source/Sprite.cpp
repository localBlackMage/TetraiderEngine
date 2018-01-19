#include "Sprite.h"
#include "../Source/Math/Vector3D.h"

using namespace JsonReader;

Sprite::Sprite() : m_xTiling(1.0f), m_yTiling(1.0f), m_hasAlpha(false) {
	type = ComponentType::Sprite;
	m_pColor = new Vector3D;
}

Sprite::~Sprite() {
	delete m_pColor;
}

void Sprite::Update(float dt) {}

void Sprite::Serialize(json j) {
	texturePath = ParseString(j, "Texture");
	m_xTiling = ParseFloat(j, "xTiling");
	m_yTiling = ParseFloat(j, "yTiling");
	m_uOffset = ParseFloat(j, "uOffset");
	m_vOffset = ParseFloat(j, "vOffset");
	m_hasAlpha = ParseBool(j, "hasAlpha");
	m_pColor->x = ParseFloat(j, "r");
	m_pColor->y = ParseFloat(j, "g");
	m_pColor->z = ParseFloat(j, "b");
	m_pColor->w = ParseFloat(j, "a");
}