#include <Stdafx.h>

RenderManager::RenderManager(int width, int height, std::string title) :
	m_la(0.1f), m_lb(0.01f), m_width(width), m_height(height), m_windowTitle(title), m_baseWindowTitle(title),
	m_pCurrentProgram(nullptr), m_debugShaderName("")
{
	_InitWindow(title);
	TETRA_EVENTS.Subscribe(EventType::EVENT_FPS_UPDATE, this);

	TETRA_EVENTS.Subscribe(EventType::EVENT_LIGHT_A_DOWN, this);
	TETRA_EVENTS.Subscribe(EventType::EVENT_LIGHT_A_UP, this);
	TETRA_EVENTS.Subscribe(EventType::EVENT_LIGHT_B_DOWN, this);
	TETRA_EVENTS.Subscribe(EventType::EVENT_LIGHT_B_UP, this);
}

RenderManager::~RenderManager() 
{
	EnableWindowsCursor();
	SDL_GL_DeleteContext(m_context);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

#pragma region Private Methods

void RenderManager::_InitWindow(std::string title)
{
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("Tetraider Engine");
	}

	SDL_Init(SDL_INIT_VIDEO);

	m_pWindow = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_width, m_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	m_context = SDL_GL_CreateContext(m_pWindow);

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "SDL Image failed to initialize." << std::endl << "Error: " << IMG_GetError() << std::endl;
	}

	// Start SDL_ttf
	if (TTF_Init() == -1) {
		std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;
	}


	SDL_SetWindowSize(m_pWindow, m_width, m_height);
	glViewport(0, 0, m_width, m_height);
}

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
	return gameObject.HasComponent(ComponentType::C_Sprite) || gameObject.HasComponent(ComponentType::C_ParticleEmitter) || gameObject.HasComponent(ComponentType::C_Text);
}

void RenderManager::_RenderSprite(const Sprite * pSpriteComp)
{
	_BindMesh(pSpriteComp->GetMesh());
	glUniform1i(SHADER_LOCATIONS::LIT, pSpriteComp->IsLit());
	glUniform2f(SHADER_LOCATIONS::FRAME_OFFSET, pSpriteComp->GetUOffset(), pSpriteComp->GetVOffset());
	glUniform2f(SHADER_LOCATIONS::FRAME_SIZE, pSpriteComp->TileX(), pSpriteComp->TileY());

	_BindUniform4(SHADER_LOCATIONS::TINT_COLOR, pSpriteComp->GetTintColor());
	_BindUniform4(SHADER_LOCATIONS::SATURATION_COLOR, pSpriteComp->GetSaturationColor());
	
	_BindUniform4(SHADER_LOCATIONS::SATURATION_COLOR, pSpriteComp->GetSaturationColor());

	if (pSpriteComp->GetAlphaMode() == GL_RGBA)
		_EnableAlphaTest();
	else
		_EnableDepthTest();

	// select the texture to use
	glBindTexture(GL_TEXTURE_2D, pSpriteComp->GetTextureBuffer());

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSpriteComp->GetMesh().GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * pSpriteComp->GetMesh().faceCount(), GL_UNSIGNED_INT, 0);
}

void RenderManager::_RenderParticles(const ParticleEmitter * pParticleEmitterComp)
{
	pParticleEmitterComp->BindBufferDatas();

	_BindMesh(pParticleEmitterComp->GetMesh());

	glUniform2f(SHADER_LOCATIONS::FRAME_SIZE, pParticleEmitterComp->FrameWidth(), pParticleEmitterComp->FrameHeight());

	_BindVertexAttribute(SHADER_LOCATIONS::P_POS_SIZE, pParticleEmitterComp->GetPositions(), 4, GL_FLOAT, GL_FALSE, 0, 0);
	_BindVertexAttribute(SHADER_LOCATIONS::P_COLOR, pParticleEmitterComp->GetColors(), 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
	_BindVertexAttribute(SHADER_LOCATIONS::P_TEXTURE_COORD, pParticleEmitterComp->GetTextureCoords(), 2, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttribDivisor(SHADER_LOCATIONS::P_POS_SIZE, 1);			// positions : one per quad (its center) -> 1
	glVertexAttribDivisor(SHADER_LOCATIONS::P_COLOR, 1);			// color : one per quad -> 1
	glVertexAttribDivisor(SHADER_LOCATIONS::P_TEXTURE_COORD, 1);	// texture coordinates : one per quad -> 1

	if (pParticleEmitterComp->GetAlphaMode() == GL_RGBA)
		_EnableAlphaTest();
	else
		_EnableDepthTest();

	glBindTexture(GL_TEXTURE_2D, pParticleEmitterComp->GetTextureBuffer());

	glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * pParticleEmitterComp->GetMesh().faceCount(), pParticleEmitterComp->LiveParticles());
}

void RenderManager::_RenderText(const Text * pTextComp, const Transform * pTransformComp)
{
	_BindMesh(pTextComp->GetMesh());

	glUniform1i(SHADER_LOCATIONS::LIT, false);
	glUniform2f(SHADER_LOCATIONS::FRAME_SIZE, pTextComp->FrameWidth(), pTextComp->FrameHeight());

	_BindUniform4(SHADER_LOCATIONS::TINT_COLOR, pTextComp->GetTintColor());
	glUniform4f(SHADER_LOCATIONS::SATURATION_COLOR, 0.f, 0.f, 0.f, 0.f);

	if (pTextComp->GetAlphaMode() == GL_RGBA)
		_EnableAlphaTest();
	else
		_EnableDepthTest();

	// select the texture to use
	glBindTexture(GL_TEXTURE_2D, pTextComp->GetTextureBuffer());


	std::vector< std::vector<TexCoords> > textureOffsets = pTextComp->GetTextureOffsets();
	ParagraphAndColors letterData = pTextComp->GetLetterData();

	int x = 0, y = 0;
	Matrix4x4 M, N;
	int triCount = 3 * pTextComp->GetMesh().faceCount();
	const GLuint faceBuffer = pTextComp->GetMesh().GetFaceBuffer();
	const float xScale = pTextComp->GetLetterWidth();
	const float yScale = pTextComp->GetLetterHeight();
	const float letterSpacing = pTextComp->GetLetterSpacing();

	Vector3D offset = pTextComp->GetOffset();

	for (Sentence letterRow : letterData.first) {
		for (Letter letter : letterRow) {
			M = pTransformComp->TransformWithOffsetAndScale(Vector3D(xScale * x * letterSpacing, yScale * y, 0) + offset, xScale, yScale);
			N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
			glUniformMatrix4fv(SHADER_LOCATIONS::MODEL_MATRIX, 1, true, (float*)M);
			glUniformMatrix4fv(SHADER_LOCATIONS::NORMAL_MATRIX, 1, true, (float*)N);

			TexCoords texOff = textureOffsets[letter.first][letter.second];
			glUniform2f(SHADER_LOCATIONS::FRAME_OFFSET, texOff.v, texOff.u);

			// draw the mesh
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
			glDrawElements(GL_TRIANGLES, triCount, GL_UNSIGNED_INT, 0);

			++x;
		}
		--y;
		x = 0;
	}
}

void RenderManager::_RenderGameObject(const GameObject& gameObject)
{
	// Only attempt to draw if the game object has a sprite component and transform component
	if (!gameObject.GetComponent<Transform>(ComponentType::C_Transform) || !_GameObjectHasRenderableComponent(gameObject))
		return;

	// set shader attributes
	if (gameObject.HasComponent(ComponentType::C_ParticleEmitter)) {
		_BindGameObjectTransform(gameObject);
		_RenderParticles(gameObject.GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter));
	}

	if (gameObject.HasComponent(ComponentType::C_Sprite)) {
		const Sprite* pSpriteComp = gameObject.GetComponent<Sprite>(ComponentType::C_Sprite);
		if (pSpriteComp->HasPosOffset())
			_BindGameObjectTransformWithOffset(gameObject, pSpriteComp->GetPosOffset());
		else 
			_BindGameObjectTransform(gameObject);

		_RenderSprite(gameObject.GetComponent<Sprite>(ComponentType::C_Sprite));
	}
	
	if (gameObject.HasComponent(ComponentType::C_Text)) {
		_BindGameObjectTransform(gameObject);
		_RenderText(gameObject.GetComponent<Text>(ComponentType::C_Text), gameObject.GetComponent<Transform>(ComponentType::C_Transform));
	}
}

void RenderManager::_SelectShaderProgram(const Component* renderingComponent)
{
	std::string shader = "";

	switch (renderingComponent->Type()) {
		case ComponentType::C_ParticleEmitter:
			shader = static_cast<const ParticleEmitter*>(renderingComponent)->Shader();
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
	glUseProgram(m_pCurrentProgram->GetProgram());

	glUniformMatrix4fv(SHADER_LOCATIONS::PERSP_MATRIX, 1, true, (float*)cameraComp->GetCameraMatrix());
	glUniformMatrix4fv(SHADER_LOCATIONS::VIEW_MATRIX, 1, true, (float*)cameraComp->GetViewMatrix());
	_BindUniform4(SHADER_LOCATIONS::CAMERA_POS, transformComp->GetPosition());
}

void RenderManager::_SetUpLights(const GameObjectLayer & gol)
{
	gol.BindBufferDatas();

	glUniform1f(SHADER_LOCATIONS::L_A, m_la);
	glUniform1f(SHADER_LOCATIONS::L_B, m_lb);
}

#pragma region Debug

void RenderManager::_SetUpDebug(const GameObject& camera)
{
	SelectShaderProgram(m_debugShaderName);
	_SetUpCamera(camera);
	_BindVertexAttribute(SHADER_LOCATIONS::POSITION, TETRA_RESOURCES.GetDebugLineMesh()->GetVertexBuffer(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
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
	Matrix4x4 SideBase = Matrix4x4::Rotate(90.f, ZAXIS) * Matrix4x4::Scale(scale.y, 0.f, 0.f);
	Matrix4x4 TopBotScale = Matrix4x4::Scale(scale.x, 0.f, 0.f);

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
	float offset = rot.z - (arcWidth / 2.f) + degreeAmt/2.f;
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

void RenderManager::_EnableAlphaTest()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.01f);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderManager::_EnableDepthTest()
{
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
}

void RenderManager::_BindMesh(const Mesh & mesh)
{
	_BindVertexAttribute(SHADER_LOCATIONS::POSITION, mesh.GetVertexBuffer(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	_BindVertexAttribute(SHADER_LOCATIONS::NORMAL, mesh.GetNormalBuffer(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	_BindVertexAttribute(SHADER_LOCATIONS::TEXTURE_COORD, mesh.GetTextCoordBuffer(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
}

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

void RenderManager::_BindUniform4(SHADER_LOCATIONS location, const Vector3D& values)
{
	glUniform4f(location, values[0], values[1], values[2], values[3]);
}

#pragma endregion

#pragma endregion

bool RenderManager::Init()
{
	// GLEW: get function bindings (if possible)
	glewInit();
	if (!GLEW_VERSION_2_0) {
		std::cout << "Needs OpenGL version 2.0 or better" << std::endl;
		return false;
	}
	return true;
}

void RenderManager::FrameStart()
{
	// clear frame buffer and z-buffer
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderManager::FrameEnd()
{
	SDL_GL_SwapWindow(m_pWindow);
}

void RenderManager::HandleEvent(Event * p_event)
{
	switch (p_event->Type()) {
	case EventType::EVENT_FPS_UPDATE:
	{
		int fps = (int)p_event->Data<FloatData>()->mValue;
		SetWindowTitle(m_baseWindowTitle + " ::: FPS: " + std::to_string(fps));
		break;
	}
	case EVENT_LIGHT_A_DOWN:
		m_la -= 0.01;
		break;
	case EVENT_LIGHT_A_UP:
		m_la += 0.01;
		break;
	case EVENT_LIGHT_B_DOWN:
		m_lb -= 0.01;
		break;
	case EVENT_LIGHT_B_UP:
		m_lb += 0.01;
		break;
	}
}

#pragma region Window Methods

void RenderManager::Resize(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
}

void RenderManager::EnableWindowsCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
}

void RenderManager::DisableWindowsCursor()
{
	SDL_ShowCursor(SDL_DISABLE);
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

void RenderManager::RenderGameObject(const GameObject& camera, const GameObject& gameObject, const GameObjectLayer& gol)
{
	//_RenderGameObject(go);


	// Only attempt to draw if the game object has a sprite component and transform component
	if (!gameObject.GetComponent<Transform>(ComponentType::C_Transform) || !_GameObjectHasRenderableComponent(gameObject))
		return;

	// set shader attributes
	if (gameObject.HasComponent(ComponentType::C_ParticleEmitter)) {
		const ParticleEmitter* cpParticleEmitterComp = gameObject.GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter);
		_SelectShaderProgram(cpParticleEmitterComp);
		_SetUpCamera(camera);
		_BindGameObjectTransform(gameObject);
		_RenderParticles(cpParticleEmitterComp);
	}

	if (gameObject.HasComponent(ComponentType::C_Sprite)) {
		const Sprite* cpSpriteComp = gameObject.GetComponent<Sprite>(ComponentType::C_Sprite);
		_SelectShaderProgram(cpSpriteComp);
		_SetUpCamera(camera);
		_SetUpLights(gol);
		if (cpSpriteComp->HasPosOffset())
			_BindGameObjectTransformWithOffset(gameObject, cpSpriteComp->GetPosOffset());
		else
			_BindGameObjectTransform(gameObject);

		_RenderSprite(gameObject.GetComponent<Sprite>(ComponentType::C_Sprite));
	}

	if (gameObject.HasComponent(ComponentType::C_Text)) {
		const Text* cpTextComp = gameObject.GetComponent<Text>(ComponentType::C_Text);
		_SelectShaderProgram(cpTextComp);
		_SetUpCamera(camera);
		_BindGameObjectTransform(gameObject);
		_RenderText(cpTextComp, gameObject.GetComponent<Transform>(ComponentType::C_Transform));
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

#pragma region Shaders

void RenderManager::LoadShaders()
{
	std::string shaderDir = TETRA_GAME_CONFIG.ShadersDir();
	// TODO: Update these
	LoadShaderProgram(shaderDir, m_debugShaderName + ".json");
	LoadShaderProgram(shaderDir, "defaultShader.json");
	LoadShaderProgram(shaderDir, "particleShader.json");
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