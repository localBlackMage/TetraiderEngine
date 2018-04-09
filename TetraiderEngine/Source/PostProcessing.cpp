#include <Stdafx.h>
#include "PostProcessing.h"

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
	if (m_pMiniMapMaskIR)	delete m_pMiniMapMaskIR;
	if (m_pMiniMapOriginalIR)	delete m_pMiniMapOriginalIR;
}

void PostProcessing::HandleEvent(Event * p_event)
{
	switch (p_event->Type()) {
		case EVENT_TOGGLE_POST_PROCESSING:
		{
			InputButtonData* data = p_event->Data<InputButtonData>();
			if (data->m_isReleased)
				Toggle();
			break;
		}
		case EventType::EVENT_WINDOW_RESIZED: {
			WindowResizedData* data = p_event->Data<WindowResizedData>();
			if (m_pBaseIR)	delete m_pBaseIR;
			m_pBaseIR = new ImageRenderer(m_pBaseShader, data->width, data->height, FBO_DEPTH_RENDER_BUFFER);
			break;
		}
	}
}

void PostProcessing::DebugInitialize()
{
	TETRA_EVENTS.Subscribe(EventType::EVENT_TOGGLE_POST_PROCESSING, this);
}

void PostProcessing::InitImageRenderers(const ImageRenderersData& metadata, const Resolution& resolution)
{
	m_pGaussianHIR = new ImageRenderer(metadata.pGausHShader, 512, 512, FBO_NONE);
	m_pGaussianVIR = new ImageRenderer(metadata.pGausVShader, 512, 512, FBO_NONE);
	m_pBaseIR = new ImageRenderer( metadata.pBaseShader, resolution.width, resolution.height, FBO_DEPTH_RENDER_BUFFER);
	m_pSecondBaseIR = new ImageRenderer(metadata.pBaseShader, 2048, 2048, FBO_NONE);
	m_pMiniMapMaskIR = new ImageRenderer(metadata.pMaskShader, 1024, 1024, FBO_NONE);
	m_pMiniMapOriginalIR = new ImageRenderer(metadata.pMiniMapShader, 1024, 1024, FBO_NONE);

	m_pBaseShader = metadata.pBaseShader;

	TETRA_EVENTS.Subscribe(EventType::EVENT_WINDOW_RESIZED, this);
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
	//m_pGaussianHIR->ClearBuffer();
	//m_pGaussianVIR->ClearBuffer();

	//m_pGaussianVIR->Render(m_pBaseIR);
	//m_pGaussianVIR->RenderToScreen(*m_pBaseShader);

	//m_pGaussianHIR->Render(m_pGaussianVIR);
	//m_pGaussianHIR->RenderToScreen(*m_pBaseShader);
	int windowWidth, windowHeight;
	TETRA_RENDERER.WindowDimensions(windowWidth, windowHeight);
	m_pBaseIR->RenderToScreen(*m_pBaseShader, windowWidth, windowHeight);

	//m_pSecondBaseIR->ClearBuffer();
	//m_pSecondBaseIR->Render(m_pGaussianHIR);

	////TETRA_RENDERER.ClearBuffer();

	////m_pSecondBaseIR->RenderToScreen(*m_pBaseShader, *m_pBaseIR);
	//m_pSecondBaseIR->RenderToScreen(*m_pBaseShader);

	_End();
}

void PostProcessing::CreateMiniMapTexture(const std::vector<RoomNodeData>& roomNodeData, unsigned short rows, unsigned short cols)
{
	// 1024 is the dimensions of the minimap texture to be drawn too in the end
	float rowHeight = 1024.f / float(rows);
	float colWidth = 1024.f / float(cols);

	// Clear all three buffers from any previous runs
	m_pMiniMapFinalIR->ClearBuffer();
	m_pMiniMapMaskIR->ClearBuffer();
	m_pMiniMapOriginalIR->ClearBuffer();

	// Bind the original MiniMap texture for drawing
	m_pMiniMapOriginalIR->BindFBO();







	glUseProgram(m_pMiniMapOriginalIR.Shader()->GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);

	Matrix4x4 M;
	const Resolution& res = TETRA_GAME_CONFIG.GetResolution();
	float aspectRatio = float(width) / float(height);

	if (!IsSimilar(aspectRatio, res.aspectRatio)) {
		int widthDiff = width - int(res.width);
		int heightDiff = height - int(res.height);
		// Closer to height
		if (abs(widthDiff) > abs(heightDiff)) {
			M = Matrix4x4::Scale(1.f + float(widthDiff) / float(res.width), 1, 1);
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

	TETRA_RENDERER.EnableAlphaTest();

	// Bind PostProcessing's base FBO and render it
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pFBO->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	glUniform1i(TEXTURE_LOCATIONS::NUM_TEXTURES, 1);

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);







	for (RoomNodeData data : roomNodeData) {
		// Scale room to rowHeight X colWidth
		// Translate room to (col*colWidth, row*rowHeight,0)
		// Draw
	}


	// Unbind the original MiniMap and leave it as is
	m_pMiniMapOriginalIR->UnbindFBO();
}