#pragma once

#ifndef FONT_INFO_H
#define FONT_INFO_H

typedef struct {
	int minX,
		maxX,
		minY,
		maxY,
		advance,
		alphaMode;
} GlyphMetrics;

struct FontInfo {
	TTF_Font * m_font;
	int m_startGlyph;
	int m_style;
	int m_kerning;
	int m_hinting;
	int m_outline;
	int m_fontSize;
	GlyphMetrics m_metrics[128];
	SDL_Surface * m_text[128];
	GLuint m_texBuffers[128];

	explicit FontInfo(TTF_Font * _font, int _startGlyph, int _fontSize) :
		m_font(_font),
		m_startGlyph(_startGlyph),
		m_style(TTF_STYLE_NORMAL),
		m_kerning(1),
		m_hinting(TTF_HINTING_NORMAL),
		m_outline(0),
		m_fontSize(_fontSize)
	{};

	~FontInfo() {
		TTF_CloseFont(m_font);

		for (int i = 0; i<128; i++)
		{
			if (m_text[i])
				SDL_FreeSurface(m_text[i]);
			m_text[i] = nullptr;
		}
	}

	/* 
		Get the m_font face attributes, if any 
	*/
	inline std::string FontFamilyName() const { return std::string(TTF_FontFaceFamilyName(m_font)); }

	/*
		Get the m_font face attributes, if any
	*/
	inline std::string FontFaceStyleName() const { return std::string(TTF_FontFaceStyleName(m_font)); }

	/*
		Check wether a glyph is provided by the m_font or not
	*/
	inline bool IsGlyphProvided() const { return TTF_GlyphIsProvided(m_font, 'g'); }

	/*
		Get the total height of this m_font - usually equal to the point size.
	*/
	inline int Height() const { return TTF_FontHeight(m_font); }

	/*
		Get the offset from the baseline to the top of the m_font.
		This is a positive value, relative to the baseline.
	*/
	inline int Ascent() const { return TTF_FontAscent(m_font); }

	/*
		Get the offset from the baseline to the bottom of the m_font
		This is a negative value, relative to the baseline.
	*/
	inline int Descent() const { return TTF_FontDescent(m_font); }

	/*
		Get the recommended spacing between lines of text for this m_font
	*/
	inline int LineSkip() const { return TTF_FontLineSkip(m_font); }
	
	/*
		Get the m_font face attributes, if any
	*/
	inline int IsFixedWidth() const { return TTF_FontFaceIsFixedWidth(m_font); }
};

#endif