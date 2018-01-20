#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "Math\Vector3D.h"
#include "Mesh.h"
#include "STBSurface.h"
#include <string>

class Sprite : public Component 
{
protected:
	std::string m_texturePath;
	
	float m_xTiling;
	float m_yTiling;
	float m_uOffset;
	float m_vOffset;
	Vector3D m_color;
	SurfaceTextureBuffer * m_texture;
	Mesh& m_mesh;

	std::string m_shader;

public:
	Sprite(std::string spriteName = "");
	~Sprite();

	virtual void Update(float dt);
	virtual void Serialize(json j);	
	virtual void Override(json j);

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
};

#endif

