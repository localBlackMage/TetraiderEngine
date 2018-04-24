/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

RenderManager::RenderManager(int width, int height, std::string title) :
	m_la(-0.24f), m_lb(0.19f), m_lights(true), m_width(width), m_height(height), m_windowTitle(title), m_baseWindowTitle(title),
	m_pCurrentProgram(nullptr), m_debugShaderName(""), m_cursorEnabled(false), 
	m_clearColor(Vector3D(0,0,0, 0.f)), m_isFullscreen(false)
{
}

RenderManager::~RenderManager()
{
	EnableWindowsCursor();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_GL_DeleteContext(m_context);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

#pragma region Private Methods

std::string RenderManager::_LoadTextFile(std::string fname)
{
	std::string out, line;
	std::ifstream in(fname);
	if (in) {
		std::getline(in, line);
		while (in) {
			out += line + "\n";
			std::getline(in, line);
		}
		return out;
	}
	else {
		std::cout << "Error reading in file: " << fname << std::endl;
		return "";
	}
}

bool RenderManager::_GameObjectHasRenderableComponent(const GameObject & gameObject)
{
	return gameObject.HasComponent(ComponentType::C_Sprite) || 
		gameObject.HasComponent(ComponentType::C_FBOSprite) ||
		gameObject.HasComponent(ComponentType::C_ParticleEmitter) || 
		gameObject.HasComponent(ComponentType::C_Text);
}

void RenderManager::_RenderFBOSprite(const FBOSprite * pFBOSpriteComp)
{
	BindMesh(pFBOSpriteComp->GetMesh());
	glUniform1i(SHADER_LOCATIONS::LIT, m_lights ? pFBOSpriteComp->IsLit() : false);
	glUniform2f(SHADER_LOCATIONS::FRAME_OFFSET, pFBOSpriteComp->GetUOffset(), pFBOSpriteComp->GetVOffset());
	glUniform2f(SHADER_LOCATIONS::FRAME_SIZE, pFBOSpriteComp->TileX(), pFBOSpriteComp->TileY());

	_BindUniform4(SHADER_LOCATIONS::TINT_COLOR, pFBOSpriteComp->GetTintColor());
	_BindUniform4(SHADER_LOCATIONS::SATURATION_COLOR, pFBOSpriteComp->GetSaturationColor());

	EnableAlphaTest();

	// select the texture to use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pFBOSpriteComp->GetTextureBuffer());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);
	GLint repeatOrClamp = /*pFBOSpriteComp->Repeats() ? GL_REPEAT : */GL_CLAMP;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatOrClamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatOrClamp);

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pFBOSpriteComp->GetMesh().GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * pFBOSpriteComp->GetMesh().faceCount(), GL_UNSIGNED_INT, 0);
}

void RenderManager::_RenderSprite(const Sprite * pSpriteComp)
{
	BindMesh(pSpriteComp->GetMesh());
	glUniform1i(SHADER_LOCATIONS::LIT, m_lights ? pSpriteComp->IsLit() : false);
	glUniform2f(SHADER_LOCATIONS::FRAME_OFFSET, pSpriteComp->GetUOffset(), pSpriteComp->GetVOffset());
	glUniform2f(SHADER_LOCATIONS::FRAME_SIZE, pSpriteComp->TileX(), pSpriteComp->TileY());

	_BindUniform4(SHADER_LOCATIONS::TINT_COLOR, pSpriteComp->GetTintColor());
	_BindUniform4(SHADER_LOCATIONS::SATURATION_COLOR, pSpriteComp->GetSaturationColor());

	if (pSpriteComp->GetAlphaMode() == GL_RGBA)
		EnableAlphaTest();
	else
		EnableDepthTest();

	// select the texture to use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pSpriteComp->GetTextureBuffer());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);
	GLint repeatOrClamp = pSpriteComp->Repeats() ? GL_REPEAT : GL_CLAMP;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatOrClamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatOrClamp);

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSpriteComp->GetMesh().GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * pSpriteComp->GetMesh().faceCount(), GL_UNSIGNED_INT, 0);
}

void RenderManager::_RenderParticles(const ParticleEmitter * pParticleEmitterComp)
{
	pParticleEmitterComp->BindBufferDatas();

	BindMesh(pParticleEmitterComp->GetMesh());

	glUniform2f(SHADER_LOCATIONS::FRAME_SIZE, pParticleEmitterComp->FrameWidth(), pParticleEmitterComp->FrameHeight());
	glUniform1f(SHADER_LOCATIONS::P_BRIGHTNESS_TINT, pParticleEmitterComp->Brightness());

	_BindVertexAttribute(SHADER_LOCATIONS::P_POS_ROT_SIZE, pParticleEmitterComp->GetPositions(), 4, GL_FLOAT, GL_FALSE, 0, 0);
	_BindVertexAttribute(SHADER_LOCATIONS::P_COLOR, pParticleEmitterComp->GetColors(), 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
	_BindVertexAttribute(SHADER_LOCATIONS::P_TEXTURE_COORD, pParticleEmitterComp->GetTextureCoords(), 2, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttribDivisor(SHADER_LOCATIONS::P_POS_ROT_SIZE, 1);			// positions : one per quad (its center) -> 1
	glVertexAttribDivisor(SHADER_LOCATIONS::P_COLOR, 1);			// color : one per quad -> 1
	glVertexAttribDivisor(SHADER_LOCATIONS::P_TEXTURE_COORD, 1);	// texture coordinates : one per quad -> 1

	if (pParticleEmitterComp->GetAlphaMode() == GL_RGBA)
		EnableAlphaTest();
	else
		EnableDepthTest();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pParticleEmitterComp->GetTextureBuffer());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * pParticleEmitterComp->GetMesh().faceCount(), pParticleEmitterComp->LiveParticles());
}

void RenderManager::_RenderText(const Text * pTextComp, const Transform * pTransformComp)
{
	BindMesh(pTextComp->GetMesh());

	glUniform1i(SHADER_LOCATIONS::LIT, false);
	glUniform2f(SHADER_LOCATIONS::FRAME_SIZE, pTextComp->FrameWidth(), pTextComp->FrameHeight());

	_BindUniform4(SHADER_LOCATIONS::TINT_COLOR, pTextComp->GetTintColor());
	glUniform4f(SHADER_LOCATIONS::SATURATION_COLOR, 0.f, 0.f, 0.f, 0.f);

	if (pTextComp->GetAlphaMode() == GL_RGBA)
		EnableAlphaTest();
	else
		EnableDepthTest();

	// select the texture to use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pTextComp->GetTextureBuffer());
	glUniform1i(TEXTURE_LOCATIONS::FIRST, 0);


	std::vector< std::vector<TexCoords> > textureOffsets = pTextComp->GetTextureOffsets();
	ParagraphAndColors letterData = pTextComp->GetLetterData();

	float letterNumber = 0.f, lineNumber = 0.f;
	Matrix4x4 M, N;
	int triCount = 3 * pTextComp->GetMesh().faceCount();
	const GLuint faceBuffer = pTextComp->GetMesh().GetFaceBuffer();
	const float letterWidth = pTextComp->GetLetterWidth();
	const float letterHeight = pTextComp->GetLetterHeight();
	const float letterSpacing = pTextComp->GetLetterSpacing();

	Vector3D offset = pTextComp->GetOffset();

	TextAlignment alignmentOption = pTextComp->GetAlignment();

	switch (alignmentOption) {
		case TextAlignment::TEXT_MIDDLE: {
			for (Sentence letterRow : letterData.first) {
				float halfLineWidth = (float(letterRow.size() - 1) * (letterWidth * letterSpacing) + letterWidth) / 2.f;

				for (Letter letter : letterRow) {
					M = pTransformComp->TransformWithOffsetAndScale(Vector3D((letterWidth * letterNumber * letterSpacing) - halfLineWidth, letterHeight * lineNumber, 0) + offset, letterWidth, letterHeight);
					N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
					glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);
					glUniformMatrix4fv(SHADER_LOCATIONS::NORMAL_MATRIX, 1, true, (float*)N);

					TexCoords texOff = textureOffsets[letter.first][letter.second];
					glUniform2f(SHADER_LOCATIONS::FRAME_OFFSET, texOff.v, texOff.u);

					// draw the mesh
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
					glDrawElements(GL_TRIANGLES, triCount, GL_UNSIGNED_INT, 0);

					++letterNumber;
				}
				--lineNumber;
				letterNumber = 0;
			}
			break;
		}

		case TextAlignment::TEXT_LEFT:
		case TextAlignment::TEXT_RIGHT: {
			for (Sentence letterRow : letterData.first) {
				for (Letter letter : letterRow) {
					M = pTransformComp->TransformWithOffsetAndScale(Vector3D(letterWidth * letterNumber * letterSpacing, letterHeight * lineNumber, 0) + offset, letterWidth, letterHeight);
					N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
					glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);
					glUniformMatrix4fv(SHADER_LOCATIONS::NORMAL_MATRIX, 1, true, (float*)N);

					TexCoords texOff = textureOffsets[letter.first][letter.second];
					glUniform2f(SHADER_LOCATIONS::FRAME_OFFSET, texOff.v, texOff.u);

					// draw the mesh
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
					glDrawElements(GL_TRIANGLES, triCount, GL_UNSIGNED_INT, 0);

					++letterNumber;
				}
				--lineNumber;
				letterNumber = 0;
			}
			break;
		}
	}
}

void RenderManager::_SelectShaderProgram(const Component* renderingComponent)
{
	std::string shader = "";

	switch (renderingComponent->Type()) {
	case ComponentType::C_ParticleEmitter:
		shader = static_cast<const ParticleEmitter*>(renderingComponent)->Shader();
		break;
	case ComponentType::C_FBOSprite:
		shader = static_cast<const FBOSprite*>(renderingComponent)->Shader();
		break;
	case ComponentType::C_Sprite:
		shader = static_cast<const Sprite*>(renderingComponent)->Shader();
		break;
	case ComponentType::C_Text:
		shader = static_cast<const Text*>(renderingComponent)->Shader();
		break;
	}

	SelectShaderProgram(shader == "" ? "default" : shader);
}

void RenderManager::_SetUpCamera(const GameObject & camera)
{
	const Camera * cameraComp = camera.GetComponent<Camera>(ComponentType::C_Camera);
	const Transform * transformComp = camera.GetComponent<Transform>(ComponentType::C_Transform);
	glUseProgram(m_pCurrentProgram->GetProgramID());

	glUniformMatrix4fv(SHADER_LOCATIONS::PERSP_MATRIX, 1, true, (float*)cameraComp->GetCameraMatrix());
	glUniformMatrix4fv(SHADER_LOCATIONS::VIEW_MATRIX, 1, true, (float*)cameraComp->GetViewMatrix());
	_BindUniform4(SHADER_LOCATIONS::CAMERA_POS, transformComp->GetPosition());
}

void RenderManager::_SetUpLights(const GameObject& gameObject, GameObjectLayer & gol)
{
	if (!m_lights) return;
	gol.BindBufferDatas(gameObject.GetComponent<Transform>(C_Transform)->GetPosition());
	_BindUniform3(SHADER_LOCATIONS::GLOBAL_AMBIENT, m_globalAmbientLight);
	glUniform1f(SHADER_LOCATIONS::L_GLOBAL_A, m_la);
	glUniform1f(SHADER_LOCATIONS::L_GLOBAL_B, m_lb);
}

#pragma region Debug

void RenderManager::_SetUpDebug(const GameObject& camera)
{
	SelectShaderProgram(m_debugShaderName);
	_SetUpCamera(camera);
	_BindVertexAttribute(SHADER_LOCATIONS::POSITION, TETRA_RESOURCES.GetDebugLineMesh()->GetVertexBuffer(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	//_BindVertexAttribute(SHADER_LOCATIONS::POSITION, TETRA_RESOURCES.GetMesh(QUAD_MESH)->GetVertexBuffer(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
}

void RenderManager::_RenderDebugCommand(DebugShape shape, const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale)
{
	switch (shape) {
	case DebugShape::S_RECT:
		_RenderRect(color, pos, rot, scale);
		break;
	case DebugShape::S_CIRCLE:
		_RenderCircle(color, scale.x / 2.f, pos);
		break;
	case DebugShape::S_LINE:
		_RenderLine(color, pos, rot, scale);
		break;
	case DebugShape::S_CONE:
		_RenderCone(color, pos, rot, scale);
		break;
	}
}

void RenderManager::_RenderRect(const Vector3D & color, const Vector3D & pos, const Vector3D & rot, const Vector3D & scale)
{
	glUniform4f(SHADER_LOCATIONS::SATURATION_COLOR, color.x, color.y, color.z, color.w);

	float halfWidth = scale.x / 2.f,
		halfHeight = scale.y / 2.f;

	// square base matrix
	Matrix4x4 Base = Matrix4x4::Translate(pos) * Matrix4x4::Rotate(rot.z, ZAXIS);
	Matrix4x4 SideBase = Matrix4x4::Rotate(90.f, ZAXIS) * Matrix4x4::Scale(scale.y, 0.f, 1.f);
	Matrix4x4 TopBotScale = Matrix4x4::Scale(scale.x, 0.f, 1.f);

	Matrix4x4 Left = Base
		* Matrix4x4::Translate(Vector3D(-halfWidth, 0, 0))
		* SideBase;
	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)Left);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	Matrix4x4 Right = Base
		* Matrix4x4::Translate(Vector3D(halfWidth, 0, 0))
		* SideBase;
	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)Right);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	Matrix4x4 Top = Base
		* Matrix4x4::Translate(Vector3D(0, halfHeight, 0))
		* TopBotScale;
	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)Top);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	Matrix4x4 Bottom = Base
		* Matrix4x4::Translate(Vector3D(0, -halfHeight, 0))
		* TopBotScale;
	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)Bottom);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}

void RenderManager::_RenderCircle(const Vector3D & color, float radius, const Vector3D & pos)
{
	glUniform4f(SHADER_LOCATIONS::SATURATION_COLOR, color.x, color.y, color.z, color.w);

	Matrix4x4 ArcMatrix;
	int max = 32;
	float degreeAmt = 360.f / float(max);
	Vector3D AXIS_Z = Vector3D(0, 0, 1);
	Vector3D a = Vector3D(radius, 0, 0);
	Vector3D b = Matrix4x4::Rotate(degreeAmt, AXIS_Z) * a;
	float lineLength = Vector3D::Distance(a, b);
	// circle base matrix
	Matrix4x4 Base = Matrix4x4::Translate(Vector3D(radius, 0, 0))
		* Matrix4x4::Rotate(90.f, ZAXIS)
		* Matrix4x4::Scale(lineLength, 0, 0);
	Matrix4x4 Position = Matrix4x4::Translate(pos);
	for (int i = 0; i < max; ++i) {
		ArcMatrix = Position
			* Matrix4x4::Rotate(degreeAmt * float(i), AXIS_Z)
			* Base;

		glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)ArcMatrix);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	}
}

void RenderManager::_RenderLine(const Vector3D & color, const Vector3D & pos, const Vector3D & rot, const Vector3D & scale)
{
	glUniform4f(SHADER_LOCATIONS::SATURATION_COLOR, color.x, color.y, color.z, color.w);

	Matrix4x4 model = Matrix4x4::Translate(pos) *
		Matrix4x4::Rotate(rot.z, ZAXIS) *
		Matrix4x4::Scale(scale.x) *
		Matrix4x4::Translate(Vector3D(.5f, 0, 0));

	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)model);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}

void RenderManager::_RenderCone(const Vector3D & color, const Vector3D & pos, const Vector3D & rot, const Vector3D & arcWidthAndRadius)
{
	float arcWidth = arcWidthAndRadius.x;
	float radius = arcWidthAndRadius.y;

	glUniform4f(SHADER_LOCATIONS::SATURATION_COLOR, color.x, color.y, color.z, color.w);

	Matrix4x4 ArcMatrix;
	int max = 32;
	float degreeAmt = arcWidth / float(max);
	float offset = rot.z - (arcWidth / 2.f) + degreeAmt / 2.f;
	//max += int(rot.z);
	Vector3D AXIS_Z = Vector3D(0, 0, 1);
	Vector3D a = Vector3D(radius, 0, 0);
	Vector3D b = Matrix4x4::Rotate(degreeAmt, AXIS_Z) * a;
	float lineLength = Vector3D::Distance(a, b);
	// Draw cone arc
	// circle base matrix
	Matrix4x4 Base = Matrix4x4::Translate(Vector3D(radius, 0, 0))
		* Matrix4x4::Rotate(90.f, ZAXIS)
		* Matrix4x4::Scale(lineLength, 0, 0);
	Matrix4x4 Position = Matrix4x4::Translate(pos);
	for (int i = 0; i < max; ++i) {
		ArcMatrix = Position
			* Matrix4x4::Rotate(degreeAmt * float(i) + offset, AXIS_Z)
			* Base;

		glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)ArcMatrix);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	}


	// Draw cone lines
	Matrix4x4 model = Matrix4x4::Translate(pos) *
		Matrix4x4::Rotate(rot.z + (arcWidth / 2.f), ZAXIS) *
		Matrix4x4::Scale(radius) *
		Matrix4x4::Translate(Vector3D(.5f, 0, 0));

	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)model);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	model = Matrix4x4::Translate(pos) *
		Matrix4x4::Rotate(rot.z - (arcWidth / 2.f), ZAXIS) *
		Matrix4x4::Scale(radius) *
		Matrix4x4::Translate(Vector3D(.5f, 0, 0));

	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)model);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}

#pragma endregion

#pragma region Binds

void RenderManager::_BindGameObjectTransform(const GameObject & gameObject)
{
	Matrix4x4 M = gameObject.GetComponent<Transform>(ComponentType::C_Transform)->GetTransform();
	Matrix4x4 N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);
	glUniformMatrix4fv(SHADER_LOCATIONS::NORMAL_MATRIX, 1, true, (float*)N);
}

void RenderManager::_BindGameObjectTransformWithOffset(const GameObject & gameObject, const Vector3D & offset)
{
	Matrix4x4 M = gameObject.GetComponent<Transform>(ComponentType::C_Transform)->GetTransformAfterOffset(offset);
	Matrix4x4 N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
	glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);
	glUniformMatrix4fv(SHADER_LOCATIONS::NORMAL_MATRIX, 1, true, (float*)N);
}

void RenderManager::_BindVertexAttribute(SHADER_LOCATIONS location, GLuint bufferID, unsigned int size, int type, int normalized, int stride, int offset)
{
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(location, size, type, normalized, stride, (void*)offset);
}

void RenderManager::_BindUniform2(SHADER_LOCATIONS location, const Vector3D& values)
{
	glUniform2f(location, values[0], values[1]);
}

void RenderManager::_BindUniform2(SHADER_LOCATIONS location, float val1, float val2)
{
	glUniform2f(location, val1, val2);
}

void RenderManager::_BindUniform3(SHADER_LOCATIONS location, const Vector3D & values)
{
	glUniform3f(location, values[0], values[1], values[2]);
}

void RenderManager::_BindUniform4(SHADER_LOCATIONS location, const Vector3D& values)
{
	glUniform4f(location, values[0], values[1], values[2], values[3]);
}

void RenderManager::_SetRendererLogicalSize(const Resolution & resolution)
{
	SDL_RenderSetLogicalSize(m_pRenderer, int(resolution.width), int(resolution.height));
}

#pragma endregion

#pragma endregion


void RenderManager::EnableAlphaTest()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_ALWAYS, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderManager::EnableAlphaTest_SourceNoDest()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_ALWAYS, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);
}

void RenderManager::EnableDepthTest()
{
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
}

void RenderManager::BindMesh(const Mesh & mesh)
{
	_BindVertexAttribute(SHADER_LOCATIONS::POSITION, mesh.GetVertexBuffer(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	_BindVertexAttribute(SHADER_LOCATIONS::NORMAL, mesh.GetNormalBuffer(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	_BindVertexAttribute(SHADER_LOCATIONS::TEXTURE_COORD, mesh.GetTextCoordBuffer(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
}

void RenderManager::ClearBuffer()
{
	ClearBuffer(m_clearColor);
}

void RenderManager::ClearBuffer(const Vector3D& color)
{
	// clear frame and depth buffers
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool RenderManager::InitGlew()
{
	// GLEW: get function bindings (if possible)
	glewInit();
	if (!GLEW_VERSION_2_0) {
		std::cout << "Needs OpenGL version 2.0 or better" << std::endl;
		return false;
	}
	FrameStart();
	FrameEnd();

	return true;
}

void RenderManager::FrameStart() 
{
	BindWindowFrameBuffer();
	ClearBuffer(m_clearColor);
}

void RenderManager::FrameEnd()
{
	SDL_GL_SwapWindow(m_pWindow);
}

void RenderManager::HandleEvent(Event * p_event)
{
	switch (p_event->Type()) {
		case EVENT_TOGGLE_LIGHTS:
		{
			if (p_event->Data<InputButtonData>()->m_isReleased)	
				m_lights = !m_lights;
			break;
		}
		case EVENT_TOGGLE_CURSOR:
		{
			if (p_event->Data<InputButtonData>()->m_isReleased) {
				m_cursorEnabled = !m_cursorEnabled;
				if (m_cursorEnabled)
					EnableWindowsCursor();
				else
					DisableWindowsCursor();
			}
			break;
		}
		case EVENT_WINDOW_RESIZED:
		{
			if (m_isFullscreen) {
				WindowResizedData * data = p_event->Data<WindowResizedData>();
				_SetRendererLogicalSize(data->resolution);
			}
			break;
		}
	}
}

#pragma region Window Methods

void RenderManager::Resize(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
}

void RenderManager::SetUpConsole()
{
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("Tetraider Engine");
	}
}

void RenderManager::InitWindow(bool debugEnabled, bool startFullScreen)
{
	if (debugEnabled) {
		TETRA_EVENTS.Subscribe(EventType::EVENT_TOGGLE_LIGHTS, this);
		TETRA_EVENTS.Subscribe(EventType::EVENT_TOGGLE_CURSOR, this);
	}
	TETRA_EVENTS.Subscribe(EventType::EVENT_WINDOW_RESIZED, this);

	SDL_Init(SDL_INIT_VIDEO);

	/* Request opengl 4 context. */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	/* Turn on double buffering with a 24bit Z buffer.
	* You may need to change this to 16 or 32 for your system */
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//m_pWindow = SDL_CreateWindow(m_windowTitle.c_str(),
	//	SDL_WINDOWPOS_CENTERED,
	//	SDL_WINDOWPOS_CENTERED,
	//	m_width, m_height,
	//	SDL_WINDOW_OPENGL);

	SDL_CreateWindowAndRenderer(
		m_width, m_height, SDL_WINDOW_OPENGL,
		&m_pWindow, &m_pRenderer);
	m_context = SDL_GL_CreateContext(m_pWindow);
	SDL_GL_SetSwapInterval(1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(m_pWindow);

	SetWindowTitle(m_windowTitle);
	SDL_SetWindowPosition(m_pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	if (startFullScreen)
		SetWindowToFullscreen();

	if (SDL_GL_SetSwapInterval(1) < 0) {
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
	}

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "SDL Image failed to initialize." << std::endl << "Error: " << IMG_GetError() << std::endl;
	}

	//// Start SDL_ttf
	//if (TTF_Init() == -1) {
	//	std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;
	//}

	SDL_SetWindowSize(m_pWindow, m_width, m_height);
	glViewport(0, 0, m_width, m_height);
}

void RenderManager::EnableWindowsCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
}

void RenderManager::DisableWindowsCursor()
{
	SDL_ShowCursor(SDL_DISABLE);
}

void RenderManager::SetWindowToFullscreen()
{
	if (!m_isFullscreen) {
		SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		m_isFullscreen = true;
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_ENTER_FULLSCREEN));
	}
}

void RenderManager::SetWindowToWindowedMode()
{
	if (m_isFullscreen) {
		SDL_SetWindowFullscreen(m_pWindow, 0);
		SDL_SetRelativeMouseMode(SDL_FALSE);
		m_isFullscreen = false;
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_LEAVE_FULLSCREEN));
	}
}

void RenderManager::SetWindowWidth(int width)
{
	m_width = width;
	glViewport(0, 0, width, m_height);
	SDL_SetWindowSize(m_pWindow, m_width, m_height);
}

void RenderManager::SetWindowHeight(int height)
{
	m_height = height;
	glViewport(0, 0, m_width, height);

	SDL_SetWindowSize(m_pWindow, m_width, m_height);
}

void RenderManager::SetWindowDimensions(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
	SDL_SetWindowSize(m_pWindow, m_width, m_height);
}

void RenderManager::SetWindowTitle(std::string title)
{
	m_windowTitle = title;
	SDL_SetWindowTitle(m_pWindow, title.c_str());
}

float RenderManager::GetAspectRatio() const
{
	return (float)m_width / (float)m_height;
}

#pragma endregion

void RenderManager::RenderGameObject(const GameObject& camera, const GameObject& gameObject, GameObjectLayer& gol)
{
	// Only attempt to draw if the game object has a renderable component and transform component
	if (!gameObject.GetComponent<Transform>(ComponentType::C_Transform) || !_GameObjectHasRenderableComponent(gameObject))
		return;

	// Render particles if they're NOT set to render on top of everything else
	if (gameObject.HasComponent(ComponentType::C_ParticleEmitter) && !gameObject.GetComponent<ParticleEmitter>(C_ParticleEmitter)->ShouldRenderLast()) {
		const ParticleEmitter* cpParticleEmitterComp = gameObject.GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter);
		_SelectShaderProgram(cpParticleEmitterComp);
		_SetUpCamera(camera);
		_BindGameObjectTransform(gameObject);
		_RenderParticles(cpParticleEmitterComp);
	}

	// Render sprite for the GO
	if (gameObject.HasComponent(ComponentType::C_Sprite)) {
		const Sprite* cpSpriteComp = gameObject.GetComponent<Sprite>(ComponentType::C_Sprite);
		_SelectShaderProgram(cpSpriteComp);
		_SetUpCamera(camera);
		_SetUpLights(gameObject, gol);
		if (cpSpriteComp->HasPosOffset())
			_BindGameObjectTransformWithOffset(gameObject, cpSpriteComp->GetPosOffset());
		else
			_BindGameObjectTransform(gameObject);

		_RenderSprite(gameObject.GetComponent<Sprite>(ComponentType::C_Sprite));
	}

	// Render fboSprite for the GO
	if (gameObject.HasComponent(ComponentType::C_FBOSprite)) {
		const FBOSprite* cpFBOSpriteComp = gameObject.GetComponent<FBOSprite>(ComponentType::C_FBOSprite);
		_SelectShaderProgram(cpFBOSpriteComp);
		_SetUpCamera(camera);
		_SetUpLights(gameObject, gol);
		if (cpFBOSpriteComp->HasPosOffset())
			_BindGameObjectTransformWithOffset(gameObject, cpFBOSpriteComp->GetPosOffset());
		else
			_BindGameObjectTransform(gameObject);

		_RenderFBOSprite(gameObject.GetComponent<FBOSprite>(ComponentType::C_FBOSprite));
	}

	// Render text for the GO
	if (gameObject.HasComponent(ComponentType::C_Text)) {
		const Text* cpTextComp = gameObject.GetComponent<Text>(ComponentType::C_Text);
		_SelectShaderProgram(cpTextComp);
		_SetUpCamera(camera);
		_BindGameObjectTransform(gameObject);
		_RenderText(cpTextComp, gameObject.GetComponent<Transform>(ComponentType::C_Transform));
	}

	// Render particles if they're set to render on top of everything else
	if (gameObject.HasComponent(ComponentType::C_ParticleEmitter) && gameObject.GetComponent<ParticleEmitter>(C_ParticleEmitter)->ShouldRenderLast()) {
		const ParticleEmitter* cpParticleEmitterComp = gameObject.GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter);
		_SelectShaderProgram(cpParticleEmitterComp);
		_SetUpCamera(camera);
		_BindGameObjectTransform(gameObject);
		_RenderParticles(cpParticleEmitterComp);
	}
}

GLuint RenderManager::GenerateStreamingVBO(unsigned int size)
{
	GLuint bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
	return bufferId;
}

GLuint RenderManager::GenerateFBO(GLuint& fboID, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type)
{
	GLuint fboTexBuffer;
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	glGenTextures(1, &fboTexBuffer);
	glBindTexture(GL_TEXTURE_2D, fboTexBuffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexBuffer, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return fboTexBuffer;
}

void RenderManager::BindWindowFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_width, m_height);
}

// Draws whatever's in the PostProcessing base FBO to the screen
void RenderManager::DrawSceneFBO()
{
	ClearBuffer(m_clearColor);

	TETRA_POST_PROCESSING.RenderBaseFBO();
}

#pragma region Shaders

void RenderManager::LoadShaders(const std::vector<std::string>& shaders)
{
	std::string shaderDir = TETRA_GAME_CONFIG.ShadersDir();
	const std::string ext = ".json";

	for (std::string shader : shaders) {
		LoadShaderProgram(shaderDir, shader + ext);
	}
}

void RenderManager::LoadShaderProgram(std::string filePath, std::string fileName)
{
	try {
		json j = JsonReader::OpenJsonFile(filePath + fileName);

		if (j.is_object()) {
			for (json::iterator it = j.begin(); it != j.end(); ++it) {
				std::string programName = it.key();
				ShaderProgram * program = CreateShaderProgram(programName);
				Shader * vShader = CreateVertexShaderFromFile(filePath + ParseString(j, programName, "vertex"));
				Shader * fShader = CreateFragmentShaderFromFile(filePath + ParseString(j, programName, "fragment"));

				program->AttachShader(*vShader);
				program->AttachShader(*fShader);
				program->LinkShaders();
			}
		}
	}
	catch (const json::parse_error& ex) {
		std::cerr << ex.what() << std::endl;
	}
}

ShaderProgram * RenderManager::GetShaderProgram(std::string programName)
{
	return m_shaderPrograms[programName];
}

ShaderProgram * RenderManager::CreateShaderProgram(std::string programName)
{
	ShaderProgram * program = m_shaderPrograms[programName];
	if (program)
		return program;

	program = new ShaderProgram();
	if (program)
		m_shaderPrograms[programName] = program;

	return program;
}

Shader * RenderManager::CreateVertexShader(std::string vertexShaderText)
{
	return new Shader(vertexShaderText, VERTEX_SHADER);
}

Shader * RenderManager::CreateVertexShaderFromFile(std::string fileName)
{
	return CreateVertexShader(_LoadTextFile(fileName));
}

Shader * RenderManager::CreateFragmentShader(std::string fragmentShaderText)
{
	return new Shader(fragmentShaderText, FRAGMENT_SHADER);

}

Shader * RenderManager::CreateFragmentShaderFromFile(std::string fileName)
{
	return CreateFragmentShader(_LoadTextFile(fileName));
}

void RenderManager::SelectShaderProgram(std::string programName)
{
	if (!m_shaderPrograms[programName]) {
		std::cout << "Shader program \"" << programName << "\" does not exist." << std::endl;
		return;
	}

	if (m_pCurrentProgram != m_shaderPrograms[programName])
		m_pCurrentProgram = m_shaderPrograms[programName];
}
#pragma endregion
