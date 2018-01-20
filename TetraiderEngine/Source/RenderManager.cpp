#include "RenderManager.h"
#include <glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "JsonReader.h"
#include "Math\Matrix4x4.h"
//#include "GameObjectManager.h"
#include "Transform.h"
#include "Camera.h"
#include "Sprite.h"

RenderManager::RenderManager(int width, int height, std::string title) :
	m_width(width), m_height(height)
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

bool RenderManager::_GameObjectHasRenderableComponent(GameObject & gameObject)
{
	return true;
	//return gameObject.Has(ComponentType::SPRITE) || gameObject.Has(ComponentType::SCROLLING_SPRITE) || gameObject.Has(ComponentType::TEXT);
}

void RenderManager::_RenderSprite(Sprite * pSpriteComp)
{
	glEnableVertexAttribArray(m_pCurrentProgram->GetAttribute("position"));
	glBindBuffer(GL_ARRAY_BUFFER, pSpriteComp->GetMesh().GetVertexBuffer());
	glVertexAttribPointer(m_pCurrentProgram->GetAttribute("position"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); // <- load it to memory

	glEnableVertexAttribArray(m_pCurrentProgram->GetAttribute("texture_coord"));
	glBindBuffer(GL_ARRAY_BUFFER, pSpriteComp->GetMesh().GetTextCoordBuffer());
	glVertexAttribPointer(m_pCurrentProgram->GetAttribute("texture_coord"), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0); // <- load it to memory

	glUniform2f(m_pCurrentProgram->GetUniform("frame_offset"), 0, 0);// pSpriteComp->GetFrameVOffset(), pSpriteComp->GetFrameUOffset());
	glUniform2f(m_pCurrentProgram->GetUniform("frame_size"), 0, 0);//pSpriteComp->FrameWidth(), pSpriteComp->FrameHeight());

	glUniform1f(m_pCurrentProgram->GetUniform("tile_x"), pSpriteComp->TileX());
	glUniform1f(m_pCurrentProgram->GetUniform("tile_y"), pSpriteComp->TileY());

	Vector3D color = pSpriteComp->GetColor();
	glUniform4f(m_pCurrentProgram->GetUniform("color"), color[0], color[1], color[2], color[3]);

	//if (pSpriteComp->TextureHasAlpha()) {
	//	glDisable(GL_DEPTH_TEST);
	//	glEnable(GL_ALPHA_TEST);
	//	glAlphaFunc(GL_GREATER, 0.4f);
	//	glEnable(GL_BLEND);
	//	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}
	//else {
	//	glDisable(GL_ALPHA_TEST);
	//	glEnable(GL_DEPTH_TEST);
	//}

	// select the texture to use
	//glBindTexture(GL_TEXTURE_2D, pSpriteComp->GetTextureBuffer());

	// draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSpriteComp->GetMesh().GetFaceBuffer());
	glDrawElements(GL_TRIANGLES, 3 * pSpriteComp->GetMesh().faceCount(), GL_UNSIGNED_INT, 0);
}

void RenderManager::_RenderGameObject(GameObject& gameObject)
{
	// Only attempt to draw if the game object has a sprite component and transform component
	if (!gameObject.GetComponent(ComponentType::Transform) || !_GameObjectHasRenderableComponent(gameObject))
		return;

	Matrix4x4 M = static_cast<Transform*>(gameObject.GetComponent(ComponentType::Transform))->GetTransform();
	Matrix4x4 N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("model_matrix"), 1, true, (float*)M);
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("normal_matrix"), 1, true, (float*)N);

	// set shader attributes
	if (gameObject.GetComponent(ComponentType::Sprite))
		_RenderSprite(static_cast<Sprite*>(gameObject.GetComponent(ComponentType::Sprite)));
}

void RenderManager::_SelectShaderProgram(GameObject & gameObject)
{
	std::string shader = "";

	//if (gameObject.Has(ComponentType::Sprite))
	//	shader = static_cast<Sprite*>(gameObject.GetComponent(ComponentType::Sprite))->Shader();

	SelectShaderProgram(shader == "" ? "default" : shader);
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
	glClearColor(0.0f, 0.0f, 1.0f, 1);
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

float RenderManager::GetAspectRatio() const
{
	return (float)m_width / (float)m_height;
}

void RenderManager::RenderGameObject(GameObject& camera, GameObject& go)
{
	if (camera == go) return;
	Camera * cameraComp = static_cast<Camera*>(camera.GetComponent(ComponentType::Camera));
	_SelectShaderProgram(go);
	glUseProgram(m_pCurrentProgram->GetProgram());

	// TODO: Update to support grabbing Perspective Matricies if needed
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("persp_matrix"), 1, true, (float*)cameraComp->GetOrthographicMatrix());
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("view_matrix"), 1, true, (float*)cameraComp->GetViewMatrix());

	_RenderGameObject(go);
}
/*
void RenderManager::RenderSTB(SurfaceTextureBuffer * pSTB, Mesh * pMesh)
{
	SelectShaderProgram("default");
	glUseProgram(m_pCurrentProgram->GetProgram());
	Matrix4x4 P = Matrix4x4::Orthographic(m_width, m_height, 0.1f);
	//Matrix4x4 P = Matrix4x4::Identity4D();
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("persp_matrix"), 1, true, (float*)&P);
	
	Matrix4x4 V = _MatrixFromCameraVectors(XAXIS, YAXIS, ZAXIS) * Matrix4x4::Translate(Vector3D(0, 0, 10));
	//Matrix4x4 V = Matrix4x4::Identity4D();
	glUniformMatrix4fv(m_pCurrentProgram->GetUniform("view_matrix"), 1, true, (float*)&V);

	/*----------------Moodie Code--------------------
	GameObjectManager& gameObjectMngr = GameObjectManager::GetInstance();
	for (auto gameObject : gameObjectMngr.mGameObjects) {
		Transform* const pTransform = static_cast<Transform*>(gameObject->GetComponent(ComponentType::Transform));
		if (!pTransform)
			continue;
		Matrix4x4 I = Matrix4x4::Identity4D();
		glUniformMatrix4fv(m_pCurrentProgram->GetUniform("model_matrix"), 1, true, (float*)pTransform->GetTransform());

		glEnableVertexAttribArray(m_pCurrentProgram->GetAttribute("position"));
		glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVertexBuffer());
		glVertexAttribPointer(m_pCurrentProgram->GetAttribute("position"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); // <- load it to memory

		glDisable(GL_ALPHA_TEST);
		glEnable(GL_DEPTH_TEST);

		// select the texture to use
		// glBindTexture(GL_TEXTURE_2D, pSTB->bufferId);

		// draw the mesh
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetFaceBuffer());
		glDrawElements(GL_TRIANGLES, 3 * pMesh->faceCount(), GL_UNSIGNED_INT, 0);
	}
}
*/

#pragma region Shaders
void RenderManager::LoadShaderProgram(std::string fileName)
{
	try {
		json j = JsonReader::OpenJsonFile(fileName);

		if (j.is_object()) {
			for (json::iterator it = j.begin(); it != j.end(); ++it) {
				std::string programName = it.key();
				ShaderProgram * program = CreateShaderProgram(programName);
				Shader * vShader = CreateVertexShaderFromFile(j[programName]["vertex"]);
				Shader * fShader = CreateFragmentShaderFromFile(j[programName]["fragment"]);

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