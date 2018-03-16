#include <Stdafx.h>

#pragma region Private Methods

void PostProcessing::_Start()
{
	glDisable(GL_DEPTH_TEST);
}

void PostProcessing::_End()
{
	glEnable(GL_DEPTH_TEST);
}

#pragma endregion

PostProcessing::PostProcessing() : 
	m_fboShader(nullptr), m_gausShader(nullptr),
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
	m_pBaseFBO = new FrameBufferObject(2048, 2048, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, FBO_NONE);
	m_pGausFBO = new FrameBufferObject(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, FBO_NONE);
}

void PostProcessing::ClearBaseFBO()
{
	m_pBaseFBO->ClearFrameBuffer();
}

void PostProcessing::BindBaseFBO()
{
	m_pBaseFBO->BindFrameBuffer();
}

void PostProcessing::UnbindBaseFBO()
{
	m_pBaseFBO->UnbindFrameBuffer();
}

void PostProcessing::ClearGausFBO()
{
	m_pGausFBO->ClearFrameBuffer();
}

void PostProcessing::BindGausFBO()
{
	m_pGausFBO->BindFrameBuffer();
}

void PostProcessing::UnbindGausFBO()
{
	m_pGausFBO->UnbindFrameBuffer();
}

void PostProcessing::DoPostProcessing()
{
	_Start();

	// Bind + clear gaus blur FBO
	m_pGausFBO->BindFrameBuffer();
	m_pGausFBO->ClearFrameBuffer();


	m_pGausFBO->UnbindFrameBuffer();

	_End();
}
