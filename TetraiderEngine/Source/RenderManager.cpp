#include "RenderManager.h"
#include "TetraiderAPI.h"
#include "JsonReader.h"
#include "DebugLineMesh.h"
#include "ShaderProgram.h"
#include "Shader.h"

#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Sprite.h"

#include <glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <windows.h>

RenderManager::RenderManager(int width, int height, std::string title) :
	m_width(width), m_height(height),
	m_pCurrentProgram(nullptr), m_debugShaderName("")
{
	_InitWindow(title);
}

RenderManager::~RenderManager() 
{
	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
}

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
	return gameObject.HasComponent(ComponentType::C_Sprite);
}

void RenderManager::_RenderSprite(const Sprite * pSpriteComp)
{
	glEnableVertexAttribArray(m_pCurrentProgram->GetAttribute("position"));
	glBindBuffer(GL_ARRAY_BUFFER, pSpriteComp->GetMesh().GetVertexBuffer());
	glVertexAttribPointer(m_pCurrentProgram->GetAttribute("position"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); // <- load it to memory

	glEnableVertexAttribArray(m_pCurrentProgram->GetAttribute("texture_coord"));
	glBindBuffer(GL_ARRAY_BUFFER, pSpriteComp->GetMesh().GetTextCoordBuffer());
	glVertexAttribPointer(m_pCurrentProgram->GetAttribute("texture_coord"), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0); // <- load it to memory

	glUniform2f(m_pCurrentProgram->GetUniform("frame_offset"), pSpriteComp->GetUOffset(), pSpriteComp->GetVOffset());// pSpriteComp->GetFrameVOffset(), pSpriteComp->GetFrameUOffset());
	glUniform2f(m_pCurrentProgram->GetUniform("frame_size"), pSpriteComp->TileX(), pSpriteComp->TileY());//pSpriteComp->FrameWidth(), pSpriteComp->FrameHeight());

	//glUniform1f(m_pCurrentProgram->GetUniform("tile_x"), 1); // pSpriteComp->TileX());
	//glUniform1f(m_pCurrentProgram->GetUniform("tile_y"), 1); // pSpriteComp->TileY());

	Vector3D color = pSpriteComp->GetColor();
	glUniform4f(m_pCurrentProgram->GetUniform("color"), color[0], color[1], color[2], color[3]);

	if (pSpriteComp->TextureHasAlpha()) {
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.4f);
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_DEPTH_TEST);
	}

	// select the texture to use
	glBindTexture(GL_TEXTURE_2D, pSpriteComp->GetTextureBuffer());

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSpriteComp->GetMesh().GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * pSpriteComp->GetMesh().faceCount(), GL_UNSIGNED_INT, 0);
}

void RenderManager::_RenderGameObject(const GameObject& gameObject)
{
	// Only attempt to draw if the game object has a sprite component and transform component
	if (!gameObject.GetComponent<Transform>(ComponentType::C_Transform) || !_GameObjectHasRenderableComponent(gameObject))
		return;

	Matrix4x4 M = gameObject.GetComponent<Transform>(ComponentType::C_Transform)->GetTransform();
	//Matrix4x4 N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("model_matrix"), 1, true, (float*)M);
	//glUniformMatrix4fv(m_pCurrentProgram->GetUniform("normal_matrix"), 1, true, (float*)N);

	// set shader attributes
	if (gameObject.HasComponent(ComponentType::C_Sprite))
		_RenderSprite(gameObject.GetComponent<Sprite>(ComponentType::C_Sprite));
}

void RenderManager::_SelectShaderProgram(const GameObject & gameObject)
{
	std::string shader = "";

	//if (gameObject.HasComponent(ComponentType::C_Sprite))
	//	shader = gameObject.GetComponent<Sprite>(ComponentType::C_Sprite))->Shader();

	SelectShaderProgram(shader == "" ? "default" : shader);
}

void RenderManager::_SetUpCamera(const GameObject & camera)
{
	const Camera * cameraComp = camera.GetComponent<Camera>(ComponentType::C_Camera);
	glUseProgram(m_pCurrentProgram->GetProgram());

	// TODO: Update to support grabbing Perspective Matricies if needed
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("persp_matrix"), 1, true, (float*)cameraComp->GetOrthographicMatrix());
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("view_matrix"), 1, true, (float*)cameraComp->GetViewMatrix());
}

void RenderManager::_SetUpDebug(const GameObject& camera)
{
	SelectShaderProgram(m_debugShaderName);
	_SetUpCamera(camera);

	glEnableVertexAttribArray(m_pCurrentProgram->GetAttribute("position"));
	glBindBuffer(GL_ARRAY_BUFFER, TETRA_RESOURCES.GetDebugLineMesh()->GetVertexBuffer());
	glVertexAttribPointer(m_pCurrentProgram->GetAttribute("position"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); // <- load it to memory
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
	}
}

void RenderManager::_RenderRect(const Vector3D & color, const Vector3D & pos, const Vector3D & rot, const Vector3D & scale)
{
	glUniform4f(m_pCurrentProgram->GetUniform("color"), color.x, color.y, color.z, color.w);

	float halfWidth = scale.x / 2.f,
		halfHeight = scale.y / 2.f;

	// square base matrix
	Matrix4x4 Base = Matrix4x4::Translate(pos) * Matrix4x4::Rotate(rot.z, Vector3D(0, 0, 1, 0));
	Matrix4x4 SideBase = Matrix4x4::Rotate(90.f, Vector3D(0, 0, 1, 0)) * Matrix4x4::Scale(scale.y, 0.f, 0.f);
	Matrix4x4 TopBotScale = Matrix4x4::Scale(scale.x, 0.f, 0.f);

	Matrix4x4 Left = Base
		* Matrix4x4::Translate(Vector3D(-halfWidth, 0, 0))
		* SideBase;
	GLint modelMatrix = m_pCurrentProgram->GetUniform("model_matrix");
	glUniformMatrix4fv(modelMatrix, 1, true, (float*)Left);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	Matrix4x4 Right = Base
		* Matrix4x4::Translate(Vector3D(halfWidth, 0, 0))
		* SideBase;
	glUniformMatrix4fv(modelMatrix, 1, true, (float*)Right);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	Matrix4x4 Top = Base
		* Matrix4x4::Translate(Vector3D(0, halfHeight, 0))
		* TopBotScale;
	glUniformMatrix4fv(modelMatrix, 1, true, (float*)Top);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	Matrix4x4 Bottom = Base
		* Matrix4x4::Translate(Vector3D(0, -halfHeight, 0))
		* TopBotScale;
	glUniformMatrix4fv(modelMatrix, 1, true, (float*)Bottom);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}

void RenderManager::_RenderCircle(const Vector3D & color, float radius, const Vector3D & pos)
{
	glUniform4f(m_pCurrentProgram->GetUniform("color"), color.x, color.y, color.z, color.w);

	Matrix4x4 ArcMatrix;
	GLint modelMatrix = m_pCurrentProgram->GetUniform("model_matrix");
	int max = 360;
	float degreeAmt = 360.f / float(max);
	Vector3D AXIS_Z = Vector3D(0, 0, 1);
	Vector3D a = pos + Vector3D(radius, 0, 0);
	Vector3D b = Matrix4x4::Rotate(degreeAmt, AXIS_Z) * a;
	float lineLength = Vector3D::Distance(a, b);
	// circle base matrix
	Matrix4x4 Base = Matrix4x4::Translate(Vector3D(radius, 0, 0))
		* Matrix4x4::Rotate(90.f, Vector3D(0, 0, 1))
		* Matrix4x4::Scale(lineLength, 0, 0);
	Matrix4x4 Position = Matrix4x4::Translate(pos);
	for (int i = 0; i < max; ++i) {
		ArcMatrix = Position
			* Matrix4x4::Rotate(degreeAmt * float(i), AXIS_Z)
			* Base;

		glUniformMatrix4fv(modelMatrix, 1, true, (float*)ArcMatrix);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	}
}

void RenderManager::_RenderLine(const Vector3D & color, const Vector3D & pos, const Vector3D & rot, const Vector3D & scale)
{
	glUniform4f(m_pCurrentProgram->GetUniform("color"), color.x, color.y, color.z, color.w);

	Matrix4x4 model = Matrix4x4::Translate(pos) * 
		Matrix4x4::Rotate(rot.z, Vector3D(0, 0, 1)) *
		Matrix4x4::Scale(scale.x) * 
		Matrix4x4::Translate(Vector3D(.5f, 0, 0));

	GLint modelMatrix = m_pCurrentProgram->GetUniform("model_matrix");
	glUniformMatrix4fv(modelMatrix, 1, true, (float*)model);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}

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
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderManager::FrameEnd()
{
	SDL_GL_SwapWindow(m_pWindow);
}

void RenderManager::Resize(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
}

void RenderManager::SetWindowWidth(int width)
{
	m_width = width;
	glViewport(0, 0, width, m_height);
}

void RenderManager::SetWindowHeight(int height)
{
	m_height = height;
	glViewport(0, 0, m_width, height);
}

void RenderManager::SetWindowTitle(std::string title)
{
	// TODO: FINISH THIS
}

float RenderManager::GetAspectRatio() const
{
	return (float)m_width / (float)m_height;
}

void RenderManager::RenderGameObject(const GameObject& camera, const GameObject& go)
{
	if (camera == go) return;
	_SelectShaderProgram(go);
	_SetUpCamera(camera);
	_RenderGameObject(go);
}

#pragma region Shaders

void RenderManager::LoadShaders()
{
	std::string shaderDir = TETRA_GAME_CONFIG.ShadersDir();

	LoadShaderProgram(shaderDir, m_debugShaderName + ".json");
	LoadShaderProgram(shaderDir, "defaultShader.json"); // TODO: Move this
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

				if (j[programName]["uniforms"].is_array()) {
					int unisLen = j[programName]["uniforms"].size();
					for (int i = 0; i < unisLen; i++) {
						program->AddUniform(j[programName]["uniforms"][i]);
					}
				}

				if (j[programName]["attributes"].is_array()) {
					int attrsLen = j[programName]["attributes"].size();
					for (int i = 0; i < attrsLen; i++) {
						program->AddAttribute(j[programName]["attributes"][i]);
					}
				}
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
	m_pCurrentProgram = m_shaderPrograms[programName];
}
#pragma endregion