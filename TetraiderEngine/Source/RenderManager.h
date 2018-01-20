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

#include <SDL.h>
#include <string>
#include <map>
#include "ShaderProgram.h"
#include "Shader.h"
#include "GameObject.h"


#include "STBSurface.h"
#include "Mesh.h"
#include "Math\Matrix4x4.h"
#include "Math\Vector3D.h"

class RenderManager
{
private:
	int m_width, m_height;
	SDL_GLContext m_context;
	SDL_Window * m_pWindow;

	std::map<std::string, ShaderProgram *> m_shaderPrograms;
	ShaderProgram * m_pCurrentProgram;

	RenderManager(int width, int height, std::string title);
	~RenderManager();

	void _InitWindow(std::string title);
	std::string _LoadTextFile(std::string fname);

public:
	RenderManager(const RenderManager &) = delete;
	void operator=(const RenderManager &) = delete;

	static RenderManager& GetInstance(int width = 800, int height = 600, std::string title = "Default Window Title")
	{
		static RenderManager instance(width, height, title);
		return instance;
	}

	bool Init();
	void FrameStart();
	void FrameEnd();
	void Resize(int width, int height);
	int WindowWidth() { return m_width; }
	int WindowHeight() { return m_height; }
	float GetAspectRatio() const;

	void RenderGameObject(GameObject& camera, GameObject& go);
	//void RenderSTB(SurfaceTextureBuffer* pSTB, Mesh* pMesh);

	void LoadShaderProgram(std::string fileName);
	ShaderProgram * GetShaderProgram(std::string programName);
	ShaderProgram * CreateShaderProgram(std::string programName);

	Shader * CreateVertexShader(std::string vertexShaderText);
	Shader * CreateVertexShaderFromFile(std::string fileName);
	Shader * CreateFragmentShader(std::string fragmentShaderText);
	Shader * CreateFragmentShaderFromFile(std::string fileName);

	void SelectShaderProgram(std::string programName);
};

#endif