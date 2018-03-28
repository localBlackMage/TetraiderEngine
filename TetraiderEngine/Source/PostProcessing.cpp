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
	m_pBaseIR(nullptr), m_pGaussianHIR(nullptr), m_pGaussianVIR(nullptr),
	m_mesh(*TETRA_RESOURCES.LoadMesh(SCREEN_QUAD_MESH))
{}

PostProcessing::~PostProcessing()
{
	if (m_pBaseIR)	delete m_pBaseIR;
	if (m_pSecondBaseIR) delete m_pSecondBaseIR;
	if (m_pGaussianHIR)	delete m_pGaussianHIR;
	if (m_pGaussianVIR)	delete m_pGaussianVIR;
}

void PostProcessing::InitImageRenderers(ImageRenderersData metadata)
{
	m_pGaussianHIR = new ImageRenderer(metadata.pGausHShader, 512, 512, FBO_NONE);
	m_pGaussianVIR = new ImageRenderer(metadata.pGausVShader, 512, 512, FBO_NONE);
	m_pBaseIR = new ImageRenderer( metadata.pBaseShader, 2048, 2048, FBO_NONE );
	m_pSecondBaseIR = new ImageRenderer(metadata.pBaseShader, 2048, 2048, FBO_NONE);

	m_pBaseShader = metadata.pBaseShader;
}

void PostProcessing::RenderBaseFBO() const
{
	m_pBaseIR->RenderToScreen(*m_pBaseShader);
}

void PostProcessing::ClearBaseFBO(const Vector3D& color)
{
	m_pBaseIR->ClearBuffer(color);
}

void PostProcessing::BindBaseFBO()
{
	m_pBaseIR->BindFBO();
}

void PostProcessing::UnbindBaseFBO()
{
	m_pBaseIR->UnbindFBO();
}

void PostProcessing::DoPostProcessing()
{
	_Start();

	// Bind + clear gaus blur FBO
	m_pGaussianHIR->ClearBuffer();
	m_pGaussianVIR->ClearBuffer();

	//m_pGaussianVIR->Render(m_pBaseIR);
	//m_pGaussianVIR->RenderToScreen(*m_pBaseShader);

	//m_pGaussianHIR->Render(m_pGaussianVIR);
	//m_pGaussianHIR->RenderToScreen(*m_pBaseShader);

	m_pBaseIR->RenderToScreen(*m_pBaseShader);

	//m_pSecondBaseIR->ClearBuffer();
	//m_pSecondBaseIR->Render(m_pGaussianHIR);

	////TETRA_RENDERER.ClearBuffer();

	////m_pSecondBaseIR->RenderToScreen(*m_pBaseShader, *m_pBaseIR);
	//m_pSecondBaseIR->RenderToScreen(*m_pBaseShader);

	_End();
}
