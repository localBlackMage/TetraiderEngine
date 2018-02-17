/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: RenderManager.h
Purpose: Provides all rendering functionality.
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Math\MathLibs.h"
#include "Subscriber.h"
#include "SurfaceTextureBuffer.h"
#include "Mesh.h"
#include <glew.h>
#include <GL/gl.h>
#include <SDL.h>
#include <string>
#include <map>
#include <algorithm>

class DebugManager;
class GameObject;
class ShaderProgram;
class Shader;
class Sprite;
class ParticleEmitter;
enum DebugShape;

//struct ParticleItem;
//struct ParticleContainer;

enum SHADER_LOCATIONS {
	POSITION = 0,
	TEXTURE_COORD,
	P_POS_SIZE,
	P_COLOR,

	PERSP_MATRIX = 10,
	VIEW_MATRIX,
	MODEL_MATRIX,
	NORMAL_MATRIX,

	TINT_COLOR = 20,
	SATURATION_COLOR,
	FRAME_OFFSET,
	FRAME_SIZE,
	TILE
};


class RenderManager : public Subscriber
{
private:
	//ParticleContainer* particleTest;

	friend class DebugManager;

	int m_width, m_height;
	std::string m_windowTitle, m_baseWindowTitle; // base window title is kinda hacky
	SDL_GLContext m_context;
	SDL_Window * m_pWindow;

	std::map<std::string, ShaderProgram *> m_shaderPrograms;
	ShaderProgram * m_pCurrentProgram;
	std::string m_debugShaderName;

	void _InitWindow(std::string title);
	std::string _LoadTextFile(std::string fname);
	bool _GameObjectHasRenderableComponent(const GameObject & gameObject);
	void _RenderSprite(const Sprite* pSpriteComp);
	void _RenderParticles(const ParticleEmitter * pParticleEmitterComp);
	void _RenderGameObject(const GameObject& gameObject);
	void _SelectShaderProgram(const GameObject& gameObject);
	void _SetUpCamera(const GameObject& camera);

	void _SetUpDebug(const GameObject& camera);
	void _RenderDebugCommand(DebugShape shape, const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderRect(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderCircle(const Vector3D & color, float radius, const Vector3D& position);
	void _RenderLine(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderCone(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& arcWidthAndRadius);
	
	void _EnableAlphaTest();
	void _EnableDepthTest();
	void _BindVertexAttribute(SHADER_LOCATIONS location, GLuint bufferID, unsigned int size, int type, int normalized, int stride = 0, int offset = 0);
public:
	RenderManager(int width = 1200, int height = 800, std::string title = "Default Window Title");
	~RenderManager();
	RenderManager(const RenderManager &) = delete;
	void operator=(const RenderManager &) = delete;

	bool Init();

	void INIT_PARTICLE_TEST();

	void FrameStart();
	void FrameEnd();
	void Resize(int width, int height);
	virtual void HandleEvent(Event * p_event);

	void SetWindowWidth(int width);
	void SetWindowHeight(int height);
	void SetWindowTitle(std::string title);
	int WindowWidth() { return m_width; }
	int WindowHeight() { return m_height; }
	float GetAspectRatio() const;
	inline SDL_Window* GetWindow() { return m_pWindow; }


	void UPDATE_PARTICLE_TEST(float deltaTime);
	void RENDER_PARTICLES_TEST(const GameObject& camera);
	void RenderGameObject(const GameObject& camera, const GameObject& go);

	GLuint GenerateStreamingVBO(unsigned int size);
	template <typename BufferType>
	void BindBufferData(const GLuint& bufferID, BufferType& bufferData, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW); // Buffer orphaning
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, bufferData);
	}


	void LoadShaders();
	void SetDebugShaderName(std::string shaderName) { m_debugShaderName = shaderName; }
	void LoadShaderProgram(std::string filePath, std::string fileName);
	ShaderProgram * GetShaderProgram(std::string programName);
	ShaderProgram * CreateShaderProgram(std::string programName);

	Shader * CreateVertexShader(std::string vertexShaderText);
	Shader * CreateVertexShaderFromFile(std::string fileName);
	Shader * CreateFragmentShader(std::string fragmentShaderText);
	Shader * CreateFragmentShaderFromFile(std::string fileName);

	void SelectShaderProgram(std::string programName);
};

#endif