#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "STBSurface.h"
#include "Mesh.h"
#include <string>

class Sprite : public Component 
{
protected:	
	float m_xTiling;
	float m_yTiling;
	float m_uOffset;
	float m_vOffset;
	Vector3D m_color;
	std::string m_textureName;
	SurfaceTextureBuffer * m_texture;
	Mesh& m_mesh;
	std::string m_shader;

public:
	Sprite(std::string textureName = "");
	~Sprite();

	virtual void Update(float dt);
	virtual void Serialize(json j);	

	const Mesh& GetMesh() const;
	void SetMesh(Mesh& mesh);

	std::string GetSpriteName() const;
	virtual void SetSprite(std::string spriteName);

	GLuint GetTextureBuffer() const;
	bool TextureHasAlpha() const;

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

	Vector3D GetColor() const { return m_color; }
	void SetColor(const Vector3D& color) { m_color = color; }
};

#endif

