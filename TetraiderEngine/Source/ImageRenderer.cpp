#include <Stdafx.h>

ImageRenderer::ImageRenderer() : 
	m_pShader(nullptr), 
	m_pFBO(nullptr),
	m_width(0),
	m_height(0)
{
}

ImageRenderer::ImageRenderer(ShaderProgram* pShader, FrameBufferObject* pFBO, GLsizei width, GLsizei height) :
	m_pShader(pShader), 
	m_pFBO(pFBO),
	m_width(width),
	m_height(height)
{}

ImageRenderer::~ImageRenderer()
{
	if (m_pFBO)	delete m_pFBO;
}
