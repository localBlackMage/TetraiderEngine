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

void PostProcessing::_PaintMiniMapMask(Vector3D playerPos)
{
#pragma region Paint Mask
	// Draw MiniMap texture and Player Pos Indicator to Final MiniMap

	m_pMiniMapMaskIR->BindFBO();

	glUseProgram(m_pMiniMapMaskIR->Shader()->GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);
	TETRA_RENDERER.EnableAlphaTest();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TETRA_RESOURCES.GetTexture("T_PlayerPos.png")->bufferId);
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	// Render player position to m_pMiniMapMaskIR
	float scaleOfDot = .2f;
	float scaleOfDotHalf = scaleOfDot / 2.f;
	Matrix4x4 scale = Matrix4x4::Scale(scaleOfDot, scaleOfDot, 1);

	playerPos.x -= scaleOfDotHalf;
	playerPos.y -= scaleOfDotHalf;

	Vector3D playerMMPos = Vector3D(playerPos.x, -playerPos.y, 0);

	Matrix4x4 M = Matrix4x4::Translate(playerMMPos) * scale;
	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);

	//// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);

	m_pMiniMapMaskIR->UnbindFBO();

#pragma endregion

#pragma region Render Mask to Screen
	//TETRA_RENDERER.SelectShaderProgram("default");
	//GameObject* cam = TETRA_GAME_OBJECTS.GetCamera(1);
	//TETRA_RENDERER._SetUpCamera(*cam);

	//TETRA_RENDERER.BindMesh(m_mesh);

	//Matrix4x4 trans = Matrix4x4::Translate(Vector3D(400, -200, 0));
	//Matrix4x4 rot = Matrix4x4::Rotate(180, ZAXIS) * Matrix4x4::Rotate(180, YAXIS);
	//scale = Matrix4x4::Scale(100, 100, 1.f);

	//M = trans*rot*scale;
	//Matrix4x4 N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
	//glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);
	//glUniformMatrix4fv(SHADER_LOCATIONS::NORMAL_MATRIX, 1, true, (float*)N);

	//glUniform1i(SHADER_LOCATIONS::LIT, false);
	//glUniform2f(SHADER_LOCATIONS::FRAME_OFFSET, 0, 0);
	//glUniform2f(SHADER_LOCATIONS::FRAME_SIZE, 1.f, 1.f);

	//TETRA_RENDERER._BindUniform4(SHADER_LOCATIONS::TINT_COLOR, Vector3D(1, 1, 1, 1));
	//TETRA_RENDERER._BindUniform4(SHADER_LOCATIONS::SATURATION_COLOR, Vector3D(0, 0, 0, 0));

	//TETRA_RENDERER.EnableAlphaTest();

	//// select the texture to use
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_pMiniMapMaskIR->FBO()->GetColorTexture());
	//glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//// draw the mesh
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	//glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);
#pragma endregion
}

void PostProcessing::_PaintMiniMap(Vector3D playerPos)
{
	// Draw MiniMap texture and Player Pos Indicator to Final MiniMap
	m_pMiniMapFinalIR->ClearBuffer();

	// Render minimap to m_pMiniMapFinalIR

	m_pMiniMapFinalIR->BindFBO();

	glUseProgram(m_pMiniMapFinalIR->Shader()->GetProgramID());
	TETRA_RENDERER.BindMesh(m_mesh);
	TETRA_RENDERER.EnableAlphaTest();

	// Bind PostProcessing's base FBO and render it
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pMiniMapMaskIR->FBO()->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::SECOND, 1);

	Matrix4x4 M = Matrix4x4::Scale(1, 1, 1);

	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);

	// Bind Origin MiniMap texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pMiniMapOriginalIR->FBO()->GetColorTexture());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);

	//glUseProgram(m_pMiniMapFinalIR->Shader()->GetProgramID());
	glUseProgram(m_pBaseShader->GetProgramID());
	TETRA_RENDERER.BindMesh(m_mesh);
	TETRA_RENDERER.EnableAlphaTest();

	// Render player position to m_pMiniMapFinalIR
	float scaleOfDot = .025f;
	float scaleOfDotHalf = scaleOfDot / 2.f;
	Matrix4x4 scale = Matrix4x4::Scale(scaleOfDot, scaleOfDot, 1);

	playerPos.x -= scaleOfDotHalf;
	playerPos.y -= scaleOfDotHalf;

	Vector3D playerMMPos = Vector3D(playerPos.x, -playerPos.y, 0);

	M = Matrix4x4::Translate(playerMMPos) * scale;
	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);

	// Bind the Player Position texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TETRA_RESOURCES.GetTexture("T_PlayerPos.png")->bufferId);
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);

	m_pMiniMapFinalIR->UnbindFBO();
}

#pragma endregion

PostProcessing::PostProcessing() : 
	m_pBaseIR(nullptr), m_pGaussianHIR(nullptr), m_pGaussianVIR(nullptr),
	m_mesh(*TETRA_RESOURCES.LoadMesh(SCREEN_QUAD_MESH)), m_shouldGenerateMiniMap(false)
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
		case EVENT_INPUT_EXITLEVEL:
		case EVENT_EXITING_GAME_LEVEL:
		{
			m_shouldGenerateMiniMap = false;
			break;
		}
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
	m_pMiniMapFinalIR = new ImageRenderer(metadata.pMiniMapFinalShader, 1024, 1024, FBO_NONE);

	m_pBaseShader = metadata.pBaseShader;

	TETRA_EVENTS.Subscribe(EventType::EVENT_WINDOW_RESIZED, this);
	TETRA_EVENTS.Subscribe(EventType::EVENT_INPUT_EXITLEVEL, this);
	TETRA_EVENTS.Subscribe(EventType::EVENT_EXITING_GAME_LEVEL, this);
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


// m_pBaseIR is not stable, do not allow it to be returned
FrameBufferObject * PostProcessing::GetImageRendererFBO(const std::string& imageRendererType) const
{
	if (imageRendererType == "secondBase")
		return m_pSecondBaseIR->FBO();
	else if (imageRendererType == "gaussianH")
			return m_pGaussianHIR->FBO();
	else if (imageRendererType == "gaussianV")
			return m_pGaussianVIR->FBO();
	else if (imageRendererType == "miniMapMask")
			return m_pMiniMapMaskIR->FBO();
	else if (imageRendererType == "miniMapOriginal")
			return m_pMiniMapOriginalIR->FBO();
	else if (imageRendererType == "miniMapFinal")
			return m_pMiniMapFinalIR->FBO();
	else
			return m_pSecondBaseIR->FBO();
}

void PostProcessing::DoPostProcessing()
{
	//_Start();
	// Bind + clear gaus blur FBO
	//m_pGaussianHIR->ClearBuffer();
	//m_pGaussianVIR->ClearBuffer();
	//m_pGaussianVIR->Render(m_pBaseIR);
	//m_pGaussianVIR->RenderToScreen(*m_pBaseShader);
	//m_pGaussianHIR->Render(m_pGaussianVIR);
	//m_pGaussianHIR->RenderToScreen(*m_pBaseShader);
	//int windowWidth, windowHeight;
	//TETRA_RENDERER.WindowDimensions(windowWidth, windowHeight);
	//_End();
}

void PostProcessing::GenerateMiniMapTextureForFrame()
{
	if (!m_shouldGenerateMiniMap)	return;

	_Start();

	Vector3D playerPos = TETRA_GAME_OBJECTS.GetPlayer()->GetComponent<Transform>(C_Transform)->GetPosition();

	playerPos.y = m_levelHeightPixels + playerPos.y;
	playerPos.x = ((playerPos.x / m_levelWidthPixels) * 2.f) - 1.f;
	playerPos.y = ((playerPos.y / m_levelHeightPixels) * 2.f) - 1.f;

	_PaintMiniMapMask(playerPos);

	_PaintMiniMap(playerPos);

	_End();
}

void PostProcessing::CreateMiniMapTexture(const std::vector<RoomNodeData>& roomNodeData, unsigned short rows, unsigned short cols, unsigned int levelWidthPixels, unsigned int levelHeightPixels)
{
	m_shouldGenerateMiniMap = true;
	m_levelWidthPixels = float(levelWidthPixels);
	m_levelHeightPixels = float(levelHeightPixels);

	_Start();
	float rowHeight = 2.f / float(rows);
	float colWidth = 2.f / float(cols);
	float halfRowHeight = rowHeight / 2.f;
	float halfColWidth = colWidth / 2.f;

	// Clear all three buffers from any previous runs
	m_pMiniMapFinalIR->ClearBuffer();
	m_pMiniMapMaskIR->ClearBuffer(Vector3D(0,0,0,1));
	m_pMiniMapOriginalIR->ClearBuffer();

	// Bind the original MiniMap texture for drawing
	m_pMiniMapOriginalIR->BindFBO();

	glUseProgram(m_pMiniMapOriginalIR->Shader()->GetProgramID());

	TETRA_RENDERER.BindMesh(m_mesh);
	TETRA_RENDERER.EnableAlphaTest();
	
	Matrix4x4 scale = Matrix4x4::Scale(halfColWidth, halfRowHeight, 1);
	for (unsigned int i = 0; i < roomNodeData.size(); ++i) {
		const RoomNodeData& data = roomNodeData[i];
		float xPos = ((float(data.col) * colWidth) + halfColWidth) - 1.f;
		float yPos = ((float(data.row) * rowHeight) + halfRowHeight) - 1.f;
		Matrix4x4 M = Matrix4x4::Translate(Vector3D(xPos, yPos, 0)) * scale;
		glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);

		// Bind PostProcessing's base FBO and render it
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, data.spriteComponent->GetTextureBuffer());
		glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

		// draw the mesh
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetFaceBuffer());
		glDrawElements(GL_TRIANGLES, 3 * m_mesh.faceCount(), GL_UNSIGNED_INT, 0);
	}
	
	// Unbind the original MiniMap and leave it as is
	m_pMiniMapOriginalIR->UnbindFBO();

	_End();
}