#include "RenderManager.h"
#include <glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "JsonReader.h"
#include "Math\Matrix4x4.h"
/*----------------Moodie code--------------*/
#include "GameObjectManager.h"
#include "Transform.h"
/*-----------------------------------------*/

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

float RenderManager::GetAspectRatio() const
{
	return (float)m_width / (float)m_height;
}

void RenderManager::RenderGameObject(GameObject& camera, GameObject& go)
{
	//CameraComponent * cComp = static_cast<CameraComponent*>(camera.Get(COMPONENT_TYPE::CAMERA));
	//_SelectShaderProgram(gameObject);
	//glUseProgram(m_pCurrentProgram->GetProgram());

	//// TODO: FIX
	//glUniformMatrix4fv(m_pCurrentProgram->GetUniform("persp_matrix"), 1, true, (float*)cComp->GetOrthographicMatrix());
	////glUniformMatrix4fv(m_pCurrentProgram->GetUniform("persp_matrix"), 1, true, (float*)cComp->GetPerspectiveMatrix());
	//glUniformMatrix4fv(m_pCurrentProgram->GetUniform("view_matrix"), 1, true, (float*)cComp->GetViewMatrix());

	//_RenderGameObject(gameObject);
}

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

	/*----------------Moodie Code--------------------*/
	GameObjectManager& gameObjectMngr = GameObjectManager::GetInstance();
	for (auto gameObject : gameObjectMngr.mGameObjects) {
		Transform* const pTransform = static_cast<Transform*>(gameObject->GetComponent(ComponentType::Transform));
		if (!pTransform)
			continue;
		Matrix4x4 I = Matrix4x4::Identity4D();
		glUniformMatrix4fv(m_pCurrentProgram->GetUniform("model_matrix"), 1, true, (float*)pTransform->m_transform);

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
	//if (!m_shaderPrograms[programName]) {
	//	std::cout << "Shader program \"" << programName.c_str() << "\" does not exist." << std::endl;
	//	return;
	//}
	m_pCurrentProgram = m_shaderPrograms[programName];
}
#pragma endregion