#include <Stdafx.h>

PostProcessing::PostProcessing() : 
	m_gbVShader(""), m_gbHShader(""),
	m_mesh(*TETRA_RESOURCES.LoadMesh("quad"))
{
}

PostProcessing::~PostProcessing()
{
	if (m_pBaseFBO)	delete m_pBaseFBO;
	if (m_pGausFBO)	delete m_pGausFBO;
}

void PostProcessing::InitFBOs()
{
	m_pBaseFBO = new FrameBufferObject(2048, 2048, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	m_pGausFBO = new FrameBufferObject(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
}

void PostProcessing::BindBaseFBO()
{
	TETRA_RENDERER.BindFBO(*m_pBaseFBO);
}

void PostProcessing::BindGuasFBO()
{
	TETRA_RENDERER.BindFBO(*m_pGausFBO);
}
