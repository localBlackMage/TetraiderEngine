#pragma once
#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

typedef std::pair<int, int> Letter;
typedef std::vector< Letter > Sentence;
typedef std::vector< Sentence > Paragraph;
typedef std::vector<Vector3D *> Colors;
typedef std::pair< Paragraph, std::vector<Vector3D *> > ParagraphAndColors;
typedef std::vector< std::vector<TexCoords> > TexCoordsGrid;

class Text : public Component 
{
protected:
	std::string m_fontName;
	std::string m_shader;
	FontInfo* m_pFont;
	SurfaceTextureBuffer * m_texture;
	TexCoordsGrid m_textureOffsets; // collection of UV coordinates for each letter
	Paragraph m_letters; // indecies into m_textureOffsets
	Colors m_colors;
	std::string m_text;
	Mesh& m_mesh;
	
	Vector3D m_tintColor;
	Vector3D m_pivotOffset;
	float m_textHeight;		// Height in pixels of each letter
	float m_textWidth;		// Width in pixels of each letter
	float m_intialTextHeight;
	float m_intialTextWidth;
	float m_letterSpacing;	// Modifier for spacing between each letter (default is 1)

	float m_rows, m_cols;
	float m_frameHeight, m_frameWidth;

	Letter _GetTextureOffsetForLetter(const char letter);
	void _SetLettersFromString();
	void _InitFontOffsets();

public:
	Text(std::string fontName = "");
	~Text() {};

	static Component* CreateInstance() { return new Text(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	
	const Mesh& GetMesh() const;
	void SetMesh(Mesh& mesh);

	inline std::string Shader() const { return m_shader; }

	virtual void SetFont(std::string fontName);
	void SetText(std::string newText);
	std::string GetText() const;

	inline Vector3D GetOffset() const { return m_pivotOffset; }
	inline void GetOffset(const Vector3D& pivotOffset) { m_pivotOffset = pivotOffset; }
	inline void SetOffset(const Vector3D& pivotOffset) { m_pivotOffset = pivotOffset; }

	GLuint GetTextureBuffer() const;
	int GetAlphaMode() const;

	inline Vector3D GetTintColor() const { return m_tintColor; }
	inline void SetTintColor(const Vector3D& tintColor) { m_tintColor = tintColor; }

	inline float GetLetterHeight() const { return m_textHeight; }
	inline float GetLetterWidth() const { return m_textWidth; }
	inline void SetLetterHeight(float h) { m_textHeight = h ; m_intialTextHeight = h;  }
	inline void SetLetterWidth(float w) { m_textWidth = w; m_intialTextWidth = w; }
	inline float GetLetterSpacing() const { return m_letterSpacing; }

	inline float FrameHeight() const { return m_frameHeight; }
	inline float FrameWidth() const { return m_frameWidth; }

	TexCoordsGrid GetTextureOffsets() const;
	ParagraphAndColors GetLetterData() const;
};

#endif

