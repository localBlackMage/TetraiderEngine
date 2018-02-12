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
class Particle;
enum DebugShape;

static const int MaxParticles = 1000;
static const Vector3D Gravity = Vector3D(0, -9.8f, 0.f);
struct ParticleItem {
	ParticleItem() : 
		pos(Vector3D()),
		speed(Vector3D(0, .5f, 0)),
		r(0), g(255), b(0), a(255),
		size(32.f), angle(0.f), weight(1.f), life(0.f), cameradistance(-1.f)
	{}
	Vector3D pos, speed;
	GLubyte r, g, b, a;
	float size, angle, weight;
	float life;
	float cameradistance;

	bool operator<(ParticleItem& that) {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

struct ParticleContainer {
	explicit ParticleContainer(Mesh& _mesh) :
		mesh(_mesh), positionsBuffer(0), colorsBuffer(0), maxLife(2.f), pCount(0), lastUsedParticle(0)
	{}

	GLuint vbo;
	Mesh& mesh;
	GLuint positionsBuffer;
	GLuint colorsBuffer;
	GLfloat positions[MaxParticles * 4];
	GLubyte colors[MaxParticles * 4];

	SurfaceTextureBuffer * m_texture;

	float maxLife;
	int pCount;
	ParticleItem particles[MaxParticles];

	int lastUsedParticle;
	int FindUnusedParticle() {
		for (int i = lastUsedParticle; i<MaxParticles; i++) {
			if (particles[i].life <= 0.f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// Cycle around to the start of the array
		for (int i = 0; i<lastUsedParticle; i++) {
			if (particles[i].life <= 0.f) {
				lastUsedParticle = i;
				return i;
			}
		}

		return -1; // All particles are taken
	}
	void Update(float deltaTime);
	void SortParticles() {
		std::sort(&particles[0], &particles[MaxParticles]);
	}
	GLuint GetTextureBuffer() const { return m_texture->bufferId; }
};

class RenderManager : public Subscriber
{
private:
	ParticleContainer*particleTest;

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
	void _RenderParticles(const Particle* pParticleComp);
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

	void UPDATE_PARTICLE_TEST(float deltaTime);
	void RENDER_PARTICLES_TEST(const GameObject& camera);
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