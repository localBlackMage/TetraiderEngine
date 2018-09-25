/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

class ImageRenderer
{
private:
	std::string m_shader;
	ShaderProgram* m_pShader;
	FrameBufferObject* m_pFBO;
	Mesh& m_mesh;

public:
	ImageRenderer();
	ImageRenderer(ShaderProgram*, GLsizei, GLsizei, FBOType);
	~ImageRenderer();
	ImageRenderer(const ImageRenderer &) = delete;
	void operator=(const ImageRenderer &) = delete;

	inline Mesh& Mesh() const { return m_mesh; }
	inline ShaderProgram* Shader() const { return m_pShader; }
	inline FrameBufferObject* FBO() const { return m_pFBO; }

	void Render(ImageRenderer * pIR) const;
	void Render(FrameBufferObject* pOtherFBO) const;

	/*
	Renders this Image Renderer's FBO to the screen with
	the supplied shader. The rendered quad is resized to match the current screen aspect ratio
	*/
	void RenderToScreen(const ShaderProgram&, int width, int height) const;

	/*
	Renders this Image Renderer's FBO to the screen with
	the supplied shader
	*/
	void RenderToScreen(const ShaderProgram&) const;

	/*
	Renders this Image Renderer's FBO + another IR's FBO to the screen with
	the supplied shader
	*/
	void RenderToScreen(const ShaderProgram&, const ImageRenderer&) const;

	/*
	Renders this Image Renderer's FBO to the supplied FBO with
	the supplied shader
	*/
	void RenderToFBO(const FrameBufferObject&, const ShaderProgram&) const;

	/*
	Renders this Image Renderer's FBO to the given Image Renderer's FBO
	with the supplied shader
	*/
	inline void RenderToIR(const ImageRenderer& ir, const ShaderProgram& shader) const { RenderToFBO(*ir.m_pFBO, shader); }

	inline void BindFBO() const { m_pFBO->BindFrameBuffer(); }
	inline void UnbindFBO() const { m_pFBO->UnbindFrameBuffer(); }

	inline void ClearBuffer(const Vector3D& color = Vector3D(0,0,0,0)) { m_pFBO->ClearFrameBuffer(color); }
};

#endif