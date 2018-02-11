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
#include <SDL.h>
#include <string>
#include <map>

class DebugManager;
class GameObject;
class ShaderProgram;
class Shader;
class Sprite;
enum DebugShape;

class RenderManager : public Subscriber
{
private:
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
	void _RenderGameObject(const GameObject& gameObject);
	void _SelectShaderProgram(const GameObject& gameObject);
	void _SetUpCamera(const GameObject& camera);

	void _SetUpDebug(const GameObject& camera);
	void _RenderDebugCommand(DebugShape shape, const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderRect(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderCircle(const Vector3D & color, float radius, const Vector3D& position);
	void _RenderLine(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderCone(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& arcWidthAndRadius);

public:
	RenderManager(int width = 1200, int height = 800, std::string title = "Default Window Title");
	~RenderManager();
	RenderManager(const RenderManager &) = delete;
	void operator=(const RenderManager &) = delete;

	bool Init();
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

	void RenderGameObject(const GameObject& camera, const GameObject& go);

	void LoadShaders();
	void SetDebugShaderName(std::string shaderName) { m_debugShaderName = shaderName; }
	void LoadShaderProgram(std::string filePath, std::string fileName);
	ShaderProgram * GetShaderProgram(std::string programName);
	ShaderProgram * CreateShaderProgram(std::string programName);

	Shader * CreateVertexShader(std::string vertexShaderText);
	Shader * CreateVertexShaderFromFile(std::string fileName);
	Shader * CreateFragmentShader(std::string fragmentShaderText);
	Shader * CreateFragmentShaderFromFile(std::string fileName);

	inline SDL_Window* GetWindow() { 
		return m_pWindow; 
	}
	void SelectShaderProgram(std::string programName);
};

#endif