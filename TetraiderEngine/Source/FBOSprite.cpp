/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

FBOSprite::FBOSprite() :
	Component(ComponentType::C_FBOSprite),
	m_repeats(false),
	m_isLit(false),
	m_xTiling(1.0f), 
	m_yTiling(1.0f), 
	m_uOffset(0.f),
	m_vOffset(0.f),
	m_tintColor(Vector3D()),
	m_saturationColor(Vector3D()),
	m_irType(""),
	m_fbo(0),
	m_mesh(*TETRA_RESOURCES.GetInternalMesh(QUAD_MESH)),
	m_shader(""),
	m_posOffset(Vector3D())
{
}

FBOSprite::~FBOSprite() {}

void FBOSprite::Deactivate() { 
	pGO = nullptr; 
}

void FBOSprite::Update(float dt) {}

void FBOSprite::Serialize(const json& j)
{
	m_repeats = ValueExists(j, "repeats") ? j["repeats"] : false;
	m_isLit = ParseBool(j, "lit");
	m_xTiling = ParseFloat(j, "tiling", "x");
	m_yTiling = ParseFloat(j, "tiling", "y");
	m_uOffset = ParseFloat(j, "uvOffset", "u");
	m_vOffset = ParseFloat(j, "uvOffset", "v");

	m_tintColor.x = j["tint"]["r"];
	m_tintColor.y = j["tint"]["g"];
	m_tintColor.z = j["tint"]["b"];
	m_tintColor.w = j["tint"]["a"];

	m_saturationColor.x = j["saturation"]["r"];
	m_saturationColor.y = j["saturation"]["g"];
	m_saturationColor.z = j["saturation"]["b"];
	m_saturationColor.w = j["saturation"]["a"];

	m_shader = ValueExists(j, "shader") ?  j["shader"] : "default";

	m_posOffset = ParseVector3D(j, "posOffset");

	m_irType = ParseString(j, "fbo");
	m_fbo = TETRA_POST_PROCESSING.GetImageRendererFBO(m_irType);
}

void FBOSprite::Override(const json& j) {
	if (ValueExists(j, "tiling")) {
		m_xTiling = ParseFloat(j, "tiling", "x");
		m_yTiling = ParseFloat(j, "tiling", "y");
	}

	if (ValueExists(j, "repeats")) {
		m_repeats = ValueExists(j, "repeats");
	}
}

const Mesh & FBOSprite::GetMesh() const {
	return m_mesh;
}

void FBOSprite::SetMesh(Mesh & mesh) {
	m_mesh = mesh;
}

std::string FBOSprite::GetFBOSpriteName() const
{
	return m_irType;
}

void FBOSprite::SetFBOSprite(std::string irType)
{
	m_irType = irType;
	m_fbo = TETRA_POST_PROCESSING.GetImageRendererFBO(m_irType);
}

GLuint FBOSprite::GetTextureBuffer() const
{
	return m_fbo->GetColorTexture();
}

int FBOSprite::GetAlphaMode() const
{
	return GL_RGBA;
}

float FBOSprite::GetUOffset() const
{
	return m_uOffset;
}

void FBOSprite::SetUOffset(float uOffset)
{
	m_uOffset = uOffset;
}

float FBOSprite::GetVOffset() const
{
	return m_vOffset;
}

void FBOSprite::SetVOffset(float vOffset)
{
	m_vOffset = vOffset;
}

float FBOSprite::TileX() const
{
	return m_xTiling;
}

void FBOSprite::SetTileX(float tile)
{
	m_xTiling = tile;
}

float FBOSprite::TileY() const
{
	return m_yTiling;
}

void FBOSprite::SetTileY(float tile)
{
	m_yTiling = tile;
}

float FBOSprite::GetAlpha() {
	return m_tintColor.w;
}

void FBOSprite::SetAlpha(float alpha) {
	m_tintColor.w = alpha;
}

String FBOSprite::Shader() const
{
	return m_shader;
}

FrameBufferObject * FBOSprite::GetTexture() const
{
	return m_fbo;
}

