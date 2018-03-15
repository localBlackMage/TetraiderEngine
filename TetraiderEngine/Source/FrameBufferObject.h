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
	~FrameBufferObject();

	inline GLuint ID() const { return m_id; }
	inline GLuint Texture() const { return m_buffer; }
	inline GLsizei Width() const { return m_width; }
	inline GLsizei Height() const { return m_height; }
	inline GLuint Buffer() const { return m_buffer; }
};

#endif