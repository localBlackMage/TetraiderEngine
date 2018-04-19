/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef FBO_SPRITE_H
#define FBO_SPRITE_H

class FBOSprite : public Component 
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
	std::string m_irType;
	FrameBufferObject * m_fbo;
	Mesh& m_mesh;
	std::string m_shader;

	Vector3D m_posOffset;

public:
	FBOSprite();
	~FBOSprite();
	static Component* CreateInstance() { return new FBOSprite(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	void Override(const json& j);

	const Mesh& GetMesh() const;
	void SetMesh(Mesh& mesh);

	inline bool Repeats() const { return m_repeats; }
	void SetIsRepeat(bool isRepeat) { m_repeats = isRepeat;	}
	inline bool IsLit() const { return m_isLit; }

	std::string GetFBOSpriteName() const;
	virtual void SetFBOSprite(std::string irType);

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

	FrameBufferObject * GetTexture() const;

	Vector3D GetTintColor() const { return m_tintColor; }
	void SetTintColor(const Vector3D& tintColor) { m_tintColor = tintColor; }

	Vector3D GetSaturationColor() const { return m_saturationColor; }
	void SetSaturationColor(const Vector3D& saturationColor) { m_saturationColor = saturationColor; }

	bool HasPosOffset() const { return m_posOffset.x != 0.f || m_posOffset.y != 0.f; }
	const Vector3D& GetPosOffset() const { return m_posOffset; }
	float GetAlpha();
	void SetAlpha(float alpha);
};

#endif

