/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Letter Text::_GetTextureOffsetForLetter(const char letter)
{
	if (letter >= 33 && letter <= 126) {
		int letNum = letter - 32;
		return std::make_pair(letNum / int(m_cols), letNum % int(m_cols));
	}
	if (letter == 10)
		return std::make_pair(-1, -1);
	return std::make_pair(0, 0);
}

void Text::_SetLettersFromString()
{
	m_letters.clear();
	std::vector< std::pair<int, int> > m_rowOfLetters;

	for (char letter : m_text) {
		std::pair<int, int> letterData = _GetTextureOffsetForLetter(letter);
		
		// newline character
		if (letterData.first == -1) {
			m_letters.push_back(m_rowOfLetters);
			m_rowOfLetters.clear();
		}
		else
			m_rowOfLetters.push_back(letterData);
	}
	m_letters.push_back(m_rowOfLetters);
}

void Text::_InitFontOffsets()
{
	m_frameHeight = (float(m_texture->surface->h) / m_rows) / float(m_texture->surface->h);
	m_frameWidth = (float(m_texture->surface->w) / m_cols) / float(m_texture->surface->w);

	m_textureOffsets.reserve(int(m_rows));
	for (int row = 0; row < m_rows; ++row) {
		m_textureOffsets.push_back(std::vector<TexCoords>());
		m_textureOffsets[row].reserve(int(m_cols));

		for (int col = 0; col < m_cols; ++col) {
			m_textureOffsets[row].push_back(TexCoords(
				float(row) * m_frameHeight,
				float(col) * m_frameWidth
			));
		}
	}
}

Text::Text(std::string fontName) :
	Component(ComponentType::C_Text),
	m_rows(1.0f),
	m_cols(1.0f),
	m_frameHeight(0.f),
	m_frameWidth(0.f),
	m_letterSpacing(1.f),
	m_fontName(fontName),
	m_texture(0),
	m_alignment(TextAlignment::TEXT_LEFT),
	m_mesh(*TETRA_RESOURCES.GetInternalMesh(QUAD_MESH)),
	m_shader("")
{}

void Text::Deactivate()
{
	pGO = nullptr;
}

void Text::Update(float dt)
{
	if (pGO->IsParented()) {
		m_textHeight = m_intialTextHeight*pGO->GetComponent<Transform>(C_Transform)->GetParentScaleY();
		m_textWidth = m_intialTextWidth*pGO->GetComponent<Transform>(C_Transform)->GetParentScaleX();
	}
}

void Text::Serialize(const json & j)
{
	m_textHeight = j["letterHeight"];
	m_textWidth = j["letterWidth"];
	m_intialTextHeight = m_textHeight;
	m_intialTextWidth = m_textWidth;
	m_letterSpacing = ValueExists(j, "spacing") ? j["spacing"] : 1.f;

	m_pivotOffset = ParseVector3D(j, "pivotOffset");
	m_tintColor = ParseColor(j, "tint");

	m_rows = j["rows"];
	m_cols = j["cols"];

	m_text = JsonReader::ValueExists(j, "text") ? JsonReader::ParseString(j, "text") : m_text;
	m_shader = ValueExists(j, "shader") ? j["shader"] : "default";

	SetFont(JsonReader::ValueExists(j, "font") ? j["font"] : m_fontName);

	if (ValueExists(j, "alignment")) {
		std::string align = j["alignment"];
		if (align == "left")		m_alignment = TextAlignment::TEXT_LEFT;
		else if (align == "right")	m_alignment = TextAlignment::TEXT_RIGHT;
		else						m_alignment = TextAlignment::TEXT_MIDDLE;
	}

	m_shader = ValueExists(j, "shader") ? j["shader"] : "default";
}

const Mesh & Text::GetMesh() const
{
	return m_mesh;
}

void Text::SetMesh(Mesh & mesh)
{
	m_mesh = mesh;
}

void Text::SetFont(std::string fontName)
{
	m_fontName = fontName;
	m_texture = TETRA_RESOURCES.GetTexture(fontName);
	_InitFontOffsets();
	_SetLettersFromString();
}

void Text::SetText(std::string newText)
{
	m_text = newText;
	_SetLettersFromString();
}

String Text::GetText() const
{
	return m_text;
}

GLuint Text::GetTextureBuffer() const
{
	return m_texture->bufferId;
}

int Text::GetAlphaMode() const
{
	return m_texture->alphaMode;
}

TexCoordsGrid Text::GetTextureOffsets() const
{
	return m_textureOffsets;
}

ParagraphAndColors Text::GetLetterData() const
{
	return std::make_pair(
		m_letters, m_colors
	);
}
