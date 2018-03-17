#pragma once

#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

class ImageRenderer
{
private:
	ShaderProgram* m_pShader;
	FrameBufferObject* m_pFBO;
	GLsizei m_width, m_height;

public:
	ImageRenderer();
	ImageRenderer(ShaderProgram* pShader, FrameBufferObject* pFBO, GLsizei width, GLsizei height);
	~ImageRenderer();
	ImageRenderer(const ImageRenderer &) = delete;
	void operator=(const ImageRenderer &) = delete;
};

#endif