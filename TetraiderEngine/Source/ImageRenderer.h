#pragma once

#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

class ImageRenderer
{
private:
	ShaderProgram* m_pShader;
	FrameBufferObject* m_pFBO;
	Mesh& m_mesh;

public:
	ImageRenderer();
	ImageRenderer(ShaderProgram* pShader, FrameBufferObject* pFBO);
	~ImageRenderer();
	ImageRenderer(const ImageRenderer &) = delete;
	void operator=(const ImageRenderer &) = delete;

	void Render(ImageRenderer * pIR) const;
	void Render(FrameBufferObject* pOtherFBO) const;
	void Render(ShaderProgram* pShader) const;

	void BindFBO();
	void UnbindFBO();

	void ClearBuffer(const Vector3D& color = Vector3D());
};

#endif