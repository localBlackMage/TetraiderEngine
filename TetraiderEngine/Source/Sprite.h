#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "SurfaceTextureBuffer.h"
#include "Mesh.h"
#include <string>

class Sprite : public Component 
{
protected:	
	float m_xTiling;
	float m_yTiling;
	float m_uOffset;
	float m_vOffset;
	Vector3D m_tintColor;
	Vector3D m_saturationColor;
	std::string m_textureName;
	SurfaceTextureBuffer * m_texture;
	Mesh& m_mesh;
	std::string m_shader;
	bool m_hasAlpha;

public:
	Sprite(std::string textureName = "");
	~Sprite();
	static Component* CreateInstance() { return new Sprite(); }
	virtual void DeActivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);

	const Mesh& GetMesh() const;
	void SetMesh(Mesh& mesh);

	std::string GetSpriteName() const;
	virtual void SetSprite(std::string spriteName);

	GLuint GetTextureBuffer() const;
	int GetAlphaMode() const;

	float GetUOffset() const;
	void SetUOffset(float uOffset);
	float GetVOffset() const;
	void SetVOffset(float vOffset);

	float TileX() const;
	void SetTileX(float tile);
	float TileY() const;
	void SetTileY(float tile);

	std::string Shader() const;

	SurfaceTextureBuffer * GetTexture() const;

	Vector3D GetTintColor() const { return m_tintColor; }
	void SetTintColor(const Vector3D& tintColor) { m_tintColor = tintColor; }

	Vector3D GetSaturationColor() const { return m_saturationColor; }
	void SetSaturationColor(const Vector3D& saturationColor) { m_saturationColor = saturationColor; }
};

#endif

