#pragma once

#ifndef FRAME_BUFFER_OBJECT_H
#define FRAME_BUFFER_OBJECT_H

class FrameBufferObject
{
private:
	GLsizei m_width, m_height;
	GLuint m_id, m_buffer;
	GLint m_internalFormat;
	GLenum m_format;
	GLenum m_type;

public:
	FrameBufferObject(GLsizei width, GLsizei height, GLint iFormat, GLenum format, GLenum type);
	~FrameBufferObject() {}

	GLuint ID() const { return m_id; }
	GLuint Texture() const { return m_buffer; }

	void Initialize();
};

#endif