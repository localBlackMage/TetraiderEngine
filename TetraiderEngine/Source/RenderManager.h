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
class Particle;
enum DebugShape;

static const int MaxParticles = 10;

struct ParticleItem {
	Vector3D pos, speed;
	unsigned char r, g, b, a;
	float size, angle, weight;
	float life;
};

struct ParticleContainer {
	ParticleContainer() : pCount(0), lastUsedParticle(0) {}

	GLuint vbo;
	GLuint positionsBuffer;
	GLuint colorsBuffer;
	GLfloat positions[MaxParticles * 4];
	GLfloat colors[MaxParticles * 4];
	int pCount;
	ParticleItem particles[MaxParticles];

	int lastUsedParticle;
	int FindUnusedParticle() {
		for (int i = lastUsedParticle; i<MaxParticles; i++) {
			if (particles[i].life < 0) {
				lastUsedParticle = i;
				return i;
			}
		}

		for (int i = 0; i<lastUsedParticle; i++) {
			if (particles[i].life < 0) {
				lastUsedParticle = i;
				return i;
			}
		}

		return 0; // All particles are taken, override the first one
	}
	void Update(float deltaTime) {
		// Generate .5 new particule each millisecond,
		// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
		// newparticles will be huge and the next frame even longer.
		int newparticles = int(deltaTime*5.f);
		if (newparticles > int(0.016f*5.f))
			newparticles = int(0.016f*5.f);

		for (int i = 0; i<MaxParticles; i++) {

			ParticleItem& p = particles[i]; // shortcut

			if (p.life > 0.0f) {

				// Decrease life
				p.life -= deltaTime;
				if (p.life > 0.0f) {

					// Simulate simple physics : gravity only, no collisions
					p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)deltaTime * 0.5f;
					p.pos += p.speed * (float)delta;
					p.cameradistance = glm::length2(p.pos - CameraPosition);
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

					// Fill the GPU buffer
					g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

					g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

					g_particule_color_data[4 * ParticlesCount + 0] = p.r;
					g_particule_color_data[4 * ParticlesCount + 1] = p.g;
					g_particule_color_data[4 * ParticlesCount + 2] = p.b;
					g_particule_color_data[4 * ParticlesCount + 3] = p.a;

				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}
	}
};

class RenderManager : public Subscriber
{
private:
	ParticleContainer particleTest;

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