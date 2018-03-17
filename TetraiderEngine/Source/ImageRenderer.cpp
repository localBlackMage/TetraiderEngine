#include <Stdafx.h>

ImageRenderer::ImageRenderer() :
	m_mesh(*TETRA_RESOURCES.GetMesh(SCREEN_QUAD_MESH)),
	m_pShader(nullptr), 
	m_pFBO(nullptr)
{
}

ImageRenderer::ImageRenderer(ShaderProgram* pShader, FrameBufferObject* pFBO) :
	m_mesh(*TETRA_RESOURCES.GetMesh(SCREEN_QUAD_MESH)),
	m_pShader(pShader), 
	m_pFBO(pFBO)
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

	TETRA_RENDERER.EnableAlphaTest();

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

void ImageRenderer::Render(ShaderProgram* pShader) const
{
	if (!m_pFBO || !pShader) return;

	TETRA_RENDERER.BindWindowFrameBuffer();

	glUseProgram(pShader->GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);

	TETRA_RENDERER.EnableAlphaTest();

	// Bind PostProcessing's base FBO and render it
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pFBO->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);
}

void ImageRenderer::BindFBO()
{
	m_pFBO->BindFrameBuffer();
}

void ImageRenderer::UnbindFBO()
{
	m_pFBO->UnbindFrameBuffer();
}

void ImageRenderer::ClearBuffer(const Vector3D& color = Vector3D())
{
	m_pFBO->ClearFrameBuffer(color);
}
