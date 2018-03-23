#pragma once
#ifndef SPRITE_H
#define SPRITE_H

class Sprite : public Component 
{
protected:	
	bool m_repeats;
	bool m_isLit;
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

	Vector3D m_posOffset;

public:
	Sprite(std::string textureName = "");
	~Sprite();
	static Component* CreateInstance() { return new Sprite(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	void Override(const json& j);

	const Mesh& GetMesh() const;
	void SetMesh(Mesh& mesh);

	inline bool Repeats() const { return m_repeats; }
	void SetIsRepeat(bool isRepeat) { m_repeats = isRepeat;	}
	inline bool IsLit() const { return m_isLit; }

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

	bool HasPosOffset() const { return m_posOffset.x != 0.f || m_posOffset.y != 0.f; }
	const Vector3D& GetPosOffset() const { return m_posOffset; }
};

#endif

