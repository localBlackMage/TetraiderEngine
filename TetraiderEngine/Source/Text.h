#pragma once
#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "Component.h"
#include "SurfaceTextureBuffer.h"
#include "Mesh.h"
#include <string>

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
	SurfaceTextureBuffer * m_texture;
	TexCoordsGrid m_textureOffsets; // collection of UV coordinates for each letter
	Paragraph m_letters; // indecies into m_textureOffsets
	Colors m_colors;
	std::string m_text;
	Mesh& m_mesh;

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

	virtual void SetFont(std::string fontName);
	void SetText(std::string newText);
	std::string GetText() const;

	GLuint GetTextureBuffer() const;
	int GetAlphaMode() const;

	float FrameHeight() const { return m_frameHeight; }
	float FrameWidth() const { return m_frameWidth; }

	TexCoordsGrid GetTextureOffsets() const;
	ParagraphAndColors GetLetterData() const;
};

#endif

