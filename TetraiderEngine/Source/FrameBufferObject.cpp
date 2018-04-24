/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#pragma region Private Methods

void FrameBufferObject::_CreateFrameBuffer()
{
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBufferObject::_CreateTextureAttachment()
{
	glGenTextures(1, &m_colorTexture);
	glBindTexture(GL_TEXTURE_2D, m_colorTexture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);
}

void FrameBufferObject::_CreateDepthBufferAttachment()
{
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
}

void FrameBufferObject::_CreateDepthTextureAttachment()
{
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
}

#pragma endregion

FrameBufferObject::FrameBufferObject(GLsizei width, GLsizei height, GLint iFormat, GLenum format, GLenum type, FBOType fboType) :
	m_width(width), m_height(height), m_frameBuffer(-1),
	m_internalFormat(iFormat), m_format(format), m_type(type)
{
	_CreateFrameBuffer();
	_CreateTextureAttachment();
	
	switch (fboType) {
	case FBO_DEPTH_RENDER_BUFFER:
		_CreateDepthBufferAttachment();
		break;
	case FBO_DEPTH_TEXTURE:
		_CreateDepthTextureAttachment();
		break;
	case FBO_NONE:
	default:
		break;
	}

	UnbindFrameBuffer();
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1, &m_frameBuffer);
	glDeleteTextures(1, &m_colorTexture);
	glDeleteTextures(1, &m_depthTexture);
	glDeleteRenderbuffers(1, &m_colorBuffer);
	glDeleteRenderbuffers(1, &m_depthBuffer);
}

void FrameBufferObject::BindToRead() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBufferObject::BindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glViewport(0, 0, m_width, m_height);
}

void FrameBufferObject::UnbindFrameBuffer() const
{
	TETRA_RENDERER.BindWindowFrameBuffer();
}

void FrameBufferObject::ClearFrameBuffer(const Vector3D& color)
{
	BindFrameBuffer();
	// clear frame and depth buffers
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	UnbindFrameBuffer();
}
