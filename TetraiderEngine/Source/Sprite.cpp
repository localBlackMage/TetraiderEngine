#include "Sprite.h"
#include "ResourceManager.h"

using namespace JsonReader;

Sprite::Sprite(String spriteName) :
	Component(ComponentType::Sprite), 
	m_xTiling(1.0f), 
	m_yTiling(1.0f), 
	m_uOffset(0.f),
	m_vOffset(0.f),
	m_color(Vector3D()),
	m_texture(ResourceManager::GetInstance().GetTexture(spriteName)),
	m_mesh(*ResourceManager::GetInstance().LoadMesh("quad")),
	m_shader("")
{
}

Sprite::~Sprite() {}

void Sprite::Update(float dt) {}

void Sprite::Serialize(json j) {
	texturePath = ParseString(j, "Texture");
	m_xTiling = ParseFloat(j, "tiling", "x");
	m_yTiling = ParseFloat(j, "tiling", "y");
	m_uOffset = ParseFloat(j, "uvOffset", "u");
	m_vOffset = ParseFloat(j, "uvOffset", "v");
	m_color = ParseColor(j, "color");
}

void Sprite::Override(json j)
{
	texturePath = ParseString(j, "Texture");
	m_xTiling = ParseFloat(j, "tiling", "x");
	m_yTiling = ParseFloat(j, "tiling", "y");
	m_uOffset = ParseFloat(j, "uvOffset", "u");
	m_vOffset = ParseFloat(j, "uvOffset", "v");
	m_color = ParseColor(j, "color");
}


const Mesh & Sprite::GetMesh() const {
	return m_mesh;
}

void Sprite::SetMesh(Mesh & mesh) {
	m_mesh = mesh;
}

String Sprite::GetSpriteName() const
{
	return m_texturePath;
}

void Sprite::SetSprite(String texturePath)
{
	m_texturePath = texturePath;
	m_texture = ResourceManager::GetInstance().GetTexture(texturePath);
}

GLuint Sprite::GetTextureBuffer() const
{
	return m_texture->bufferId;
}

bool Sprite::TextureHasAlpha() const
{
	return m_texture->surface->hasAlpha;
}

float Sprite::GetUOffset() const
{
	return 0; //return m_frameY * m_texture->surface->frameHeight;
}

float Sprite::GetVOffset() const
{
	return 0; //return m_frameX * m_texture->surface->frameWidth;
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
