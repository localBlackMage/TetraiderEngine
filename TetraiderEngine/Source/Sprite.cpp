#include <Stdafx.h>

Sprite::Sprite(std::string textureName) :
	Component(ComponentType::C_Sprite),
	m_repeats(false),
	m_isLit(false),
	m_xTiling(1.0f), 
	m_yTiling(1.0f), 
	m_uOffset(0.f),
	m_vOffset(0.f),
	m_tintColor(Vector3D()),
	m_saturationColor(Vector3D()),
	m_textureName(textureName),
	m_texture(0),
	m_mesh(*TETRA_RESOURCES.LoadMesh("quad")),
	m_shader(""),
	m_posOffset(Vector3D())
{
}

Sprite::~Sprite() {}

void Sprite::Deactivate() { 
	pGO = nullptr; 
}

void Sprite::Update(float dt) {}

void Sprite::Serialize(const json& j)
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

	m_textureName = ParseString(j, "Texture");
	m_texture = TETRA_RESOURCES.GetTexture(m_textureName);
}

const Mesh & Sprite::GetMesh() const {
	return m_mesh;
}

void Sprite::SetMesh(Mesh & mesh) {
	m_mesh = mesh;
}

String Sprite::GetSpriteName() const
{
	return m_textureName;
}

void Sprite::SetSprite(std::string textureName)
{
	m_textureName = textureName;
	m_texture = TETRA_RESOURCES.GetTexture(m_textureName);
}

GLuint Sprite::GetTextureBuffer() const
{
	return m_texture->bufferId;
}

int Sprite::GetAlphaMode() const
{
	return m_texture->alphaMode;
}

float Sprite::GetUOffset() const
{
	return m_uOffset; //return m_frameY * m_texture->surface->frameHeight;
}

void Sprite::SetUOffset(float uOffset)
{
	m_uOffset = uOffset;
}

float Sprite::GetVOffset() const
{
	return m_vOffset; //return m_frameX * m_texture->surface->frameWidth;
}

void Sprite::SetVOffset(float vOffset)
{
	m_vOffset = vOffset;
}

float Sprite::TileX() const
{
	return m_xTiling;
}

void Sprite::SetTileX(float tile)
{
	m_xTiling = tile;
}

float Sprite::TileY() const
{
	return m_yTiling;
}

void Sprite::SetTileY(float tile)
{
	m_yTiling = tile;
}

String Sprite::Shader() const
{
	return m_shader;
}

SurfaceTextureBuffer * Sprite::GetTexture() const
{
	return m_texture;
}
