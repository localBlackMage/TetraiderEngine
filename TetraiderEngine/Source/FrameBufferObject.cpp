#include <Stdafx.h>

FrameBufferObject::FrameBufferObject(GLsizei width, GLsizei height, GLint iFormat, GLenum format, GLenum type) :
	m_width(width), m_height(height), m_id(-1), m_buffer(-1),
	m_internalFormat(iFormat), m_format(format), m_type(type)
{
	m_buffer = TETRA_RENDERER.GenerateFBO(m_id, m_internalFormat, m_width, m_height, m_format, m_type);
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1, &m_id);
	glDeleteTextures(1, &m_buffer);
}
