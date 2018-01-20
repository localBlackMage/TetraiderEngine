#include "Sprite.h"

using namespace JsonReader;

Sprite::Sprite() : 
	Component(ComponentType::Sprite), 
	m_xTiling(1.0f), m_yTiling(1.0f), 
	m_hasAlpha(false), m_color(Vector3D())
{
}

Sprite::~Sprite() {
}

void Sprite::Update(float dt) {}

void Sprite::Serialize(json j) {
	texturePath = ParseString(j, "Texture");
	m_xTiling = ParseFloat(j, "tiling", "x");
	m_yTiling = ParseFloat(j, "tiling", "y");
	m_uOffset = ParseFloat(j, "uvOffset", "u");
	m_vOffset = ParseFloat(j, "uvOffset", "v");
	m_hasAlpha = ParseBool(j, "hasAlpha");
	m_color = ParseColor(j, "color");
}