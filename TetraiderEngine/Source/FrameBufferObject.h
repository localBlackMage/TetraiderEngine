/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef FRAME_BUFFER_OBJECT_H
#define FRAME_BUFFER_OBJECT_H

enum FBOType {
	FBO_NONE = 0,
	FBO_DEPTH_RENDER_BUFFER,
	FBO_DEPTH_TEXTURE
};

class FrameBufferObject
{
private:
	GLsizei m_width, m_height;

	GLuint m_frameBuffer;

	GLuint m_colorTexture;
	GLuint m_depthTexture;

	GLuint m_colorBuffer;
	GLuint m_depthBuffer;

	GLint m_internalFormat;
	GLenum m_format;
	GLenum m_type;

	/* 
		Creates a new frame buffer object and sets the buffer to which drawing
		will occur - colour attachment 0. This is the attachment where the colour
		buffer texture is.
	*/
	void _CreateFrameBuffer();

	// Creates a texture and sets it as the colour buffer attachment for this FBO.
	void _CreateTextureAttachment();

	// Adds a depth buffer to the FBO in the form of a texture, which can later be sampled.
	void _CreateDepthBufferAttachment();

	/*
		Adds a depth buffer to the FBO in the form of a render buffer. 
		This can't be used for sampling in the shaders.
	*/
	void _CreateDepthTextureAttachment();

public:
	friend class RenderManager;
	friend class PostProcessing;

	FrameBufferObject(GLsizei width, GLsizei height, GLint iFormat, GLenum format, GLenum type, FBOType fboType);
	~FrameBufferObject();

	inline GLuint GetColorTexture() const { return m_colorTexture; }
	inline GLuint GetDepthTexture() const { return m_depthTexture; }

	// Binds the current FBO to be read from
	void BindToRead() const;

	/*
		Binds the frame buffer, setting it as the current render target. Anything
		rendered after this will be rendered to this FBO, and not to the screen.
	*/
	void BindFrameBuffer() const;

	/*
		Unbinds the frame buffer, setting the default frame buffer as the current
		render target. Anything rendered after this will be rendered to the
		screen, and not this FBO.
	*/
	void UnbindFrameBuffer() const;

	void ClearFrameBuffer(const Vector3D& color = Vector3D(0,0,0,0));

	inline GLsizei Width() const { return m_width; }
	inline GLsizei Height() const { return m_height; }
};

#endif