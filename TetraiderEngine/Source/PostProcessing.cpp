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
	if (m_pGaussianHIR)	delete m_pGaussianHIR;
	if (m_pGaussianVIR)	delete m_pGaussianVIR;
}

void PostProcessing::InitImageRenderers(ImageRenderersData metadata)
{
	m_pGaussianHIR = new ImageRenderer(
		metadata.pGausHShader,
		new FrameBufferObject(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, FBO_NONE)
	);

	m_pGaussianVIR = new ImageRenderer(
		metadata.pGausVShader,
		new FrameBufferObject(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, FBO_NONE)
	);

	m_pBaseIR = new ImageRenderer(
		metadata.pBaseShader,
		new FrameBufferObject(2048, 2048, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, FBO_DEPTH_RENDER_BUFFER)
	);

	m_pBaseShader = metadata.pBaseShader;
}

//void PostProcessing::InitFBOs()
//{
//	//m_pBaseFBO = new FrameBufferObject(2048, 2048, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, FBO_NONE);
//	//m_pGausFBO = new FrameBufferObject(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, FBO_NONE);
//}

void PostProcessing::RenderBaseFBO() const
{
	m_pBaseIR->Render(m_pBaseShader);
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

//void PostProcessing::ClearGausFBO(const Vector3D& color)
//{
//	m_pGausFBO->ClearFrameBuffer(color);
//}
//
//void PostProcessing::BindGausFBO()
//{
//	m_pGausFBO->BindFrameBuffer();
//}
//
//void PostProcessing::UnbindGausFBO()
//{
//	m_pGausFBO->UnbindFrameBuffer();
//}

void PostProcessing::DoPostProcessing()
{
	_Start();

	// Bind + clear gaus blur FBO
	m_pGaussianHIR->ClearBuffer();
	m_pGaussianVIR->ClearBuffer();

	m_pGaussianHIR->Render(m_pBaseIR);
	m_pGaussianVIR->Render(m_pGaussianHIR);
	
	//m_pGausFBO->BindFrameBuffer();
	//m_pGausFBO->ClearFrameBuffer();

	////EnableGBHShader();
	////TETRA_RENDERER._BindMesh(m_mesh);
	////TETRA_RENDERER._EnableAlphaTest();

	////glActiveTexture(GL_TEXTURE0);
	////glBindTexture(GL_TEXTURE_2D, m_pBaseFBO->m_frameBuffer);
	////glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);


	//m_pGausFBO->UnbindFrameBuffer();

	_End();
}
