/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ImageRenderer::ImageRenderer() :
	m_mesh(*TETRA_RESOURCES.GetInternalMesh(SCREEN_QUAD_MESH)),
	m_pShader(nullptr), 
	m_pFBO(nullptr)
{
}

ImageRenderer::ImageRenderer(ShaderProgram* shader, GLsizei width, GLsizei height, FBOType fboType) :
	m_pShader(shader),
	m_pFBO(new FrameBufferObject(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, fboType)),
	m_mesh(*TETRA_RESOURCES.GetInternalMesh(SCREEN_QUAD_MESH))
{}

ImageRenderer::~ImageRenderer()
{
	if (m_pFBO)	delete m_pFBO;
}

void ImageRenderer::Render(ImageRenderer * pIR) const
{
	Render(pIR->m_pFBO);
}

void ImageRenderer::Render(FrameBufferObject * pOtherFBO) const
{
	if (!m_pFBO || !m_pShader) return;

	m_pFBO->BindFrameBuffer();

	glUseProgram(m_pShader->GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);

	TETRA_RENDERER.EnableAlphaTest_SourceNoDest();

	// Bind PostProcessing's base FBO and render it
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pOtherFBO->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	glUniform1f(SHADER_LOCATIONS::TARGET_WIDTH, GLfloat(pOtherFBO->Width()));
	glUniform1f(SHADER_LOCATIONS::TARGET_HEIGHT, GLfloat(pOtherFBO->Height()));

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);

	m_pFBO->UnbindFrameBuffer();
}

void ImageRenderer::RenderToScreen(const ShaderProgram & shader, int width, int height) const
{
	TETRA_RENDERER.BindWindowFrameBuffer();

	glUseProgram(shader.GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);

	Matrix4x4 M;
	const Resolution& res = TETRA_GAME_CONFIG.GetResolution();
	float aspectRatio = float(width) / float(height);

	if (!IsSimilar(aspectRatio, res.aspectRatio)) {
		int widthDiff = width - int(res.width);
		int heightDiff = height - int(res.height);
		// Closer to height
		if (abs(widthDiff) > abs(heightDiff)) {
			M = Matrix4x4::Scale(1.f + float(widthDiff) / float(res.width) , 1, 1);
		}
		// Closer to width
		else {
			M = Matrix4x4::Scale(1, 1.f + float(heightDiff) / float(res.height), 1);
		}
	}
	// Close enough to the desired aspect ratio, just use it
	else 
		M = Matrix4x4::Scale(1, 1, 1);

	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);

	TETRA_RENDERER.EnableAlphaTest_SourceNoDest();

	// Bind PostProcessing's base FBO and render it
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pFBO->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	glUniform1i(TEXTURE_LOCATIONS::NUM_TEXTURES, 1);

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);
}

void ImageRenderer::RenderToScreen(const ShaderProgram& shader) const
{
	TETRA_RENDERER.BindWindowFrameBuffer();

	glUseProgram(shader.GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);

	TETRA_RENDERER.EnableAlphaTest_SourceNoDest();

	// Bind PostProcessing's base FBO and render it
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pFBO->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	glUniform1i(TEXTURE_LOCATIONS::NUM_TEXTURES, 1);

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);
}

void ImageRenderer::RenderToScreen(const ShaderProgram & shader, const ImageRenderer & ir) const
{
	TETRA_RENDERER.BindWindowFrameBuffer();

	glUseProgram(shader.GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);

	TETRA_RENDERER.EnableAlphaTest_SourceNoDest();

	// Bind PostProcessing's base FBO and render it
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pFBO->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ir.m_pFBO->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::SECOND, 1);

	glUniform1i(TEXTURE_LOCATIONS::NUM_TEXTURES, 2);

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);
}

void ImageRenderer::RenderToFBO(const FrameBufferObject & fbo, const ShaderProgram & shader) const
{
	fbo.BindFrameBuffer();

	glUseProgram(shader.GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);

	TETRA_RENDERER.EnableAlphaTest();

	// Bind PostProcessing's base FBO and render it
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pFBO->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 & m_mesh.faceCount(), GL_UNSIGNED_INT, 0);

	fbo.UnbindFrameBuffer();
}

void ImageRenderer::RenderToIR(const ImageRenderer & ir, const ShaderProgram & shader) const
{
	RenderToFBO(*ir.m_pFBO, shader);
}

void ImageRenderer::BindFBO()
{
	m_pFBO->BindFrameBuffer();
}

void ImageRenderer::UnbindFBO()
{
	m_pFBO->UnbindFrameBuffer();
}

void ImageRenderer::ClearBuffer(const Vector3D& color)
{
	m_pFBO->ClearFrameBuffer(color);
}
