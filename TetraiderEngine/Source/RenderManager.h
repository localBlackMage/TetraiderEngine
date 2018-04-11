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

enum TEXTURE_LOCATIONS {
	FIRST = 0,
	SECOND = 1,
	THIRD = 2,
	FOURTH = 3,
	NUM_TEXTURES = 4
};

enum SHADER_LOCATIONS {
	POSITION = 0,		// 0
	NORMAL,				// 1
	TEXTURE_COORD,		// 2
	P_POS_ROT_SIZE,		// 3
	P_COLOR,			// 4
	P_TEXTURE_COORD,	// 5

	PERSP_MATRIX = 10,	// 10
	VIEW_MATRIX,		// 11
	MODEL_MATRIX,		// 12
	NORMAL_MATRIX,		// 13
	CAMERA_POS,			// 14

	TINT_COLOR = 30,	// 30
	SATURATION_COLOR,	// 31
	FRAME_OFFSET,		// 32
	FRAME_SIZE,			// 33
	TILE,				// 34
	TARGET_WIDTH,		// 35
	TARGET_HEIGHT,		// 36
	P_BRIGHTNESS_TINT,	// 37

	GLOBAL_AMBIENT = 40,// 40
	AMBIENT,			// 41
	DIFFUSE,			// 42
	SPECULAR,			// 43

	LIT = 47,			// 47
	L_GLOBAL_A = 48,	// 48
	L_GLOBAL_B = 49,	// 49
	L_POS_DIST = 50,	// 50
	L_COLOR = L_POS_DIST+MAX_LIGHTS,		// 70
	L_AB_VALUES = L_COLOR+MAX_LIGHTS		// 90

};

class GameObjectLayer;


class RenderManager : public Subscriber
{
private:
	friend class DebugManager;
	friend class PostProcessing;
	friend class GameObjectManager;

	float m_la, m_lb;		// light falloff numbers
	bool m_lights;			// whether or not lights should be rendered
	Vector3D m_globalAmbientLight;
	int m_width, m_height;
	std::string m_windowTitle, m_baseWindowTitle; // base window title is kinda hacky
	SDL_GLContext m_context;
	SDL_Window * m_pWindow;
	bool m_cursorEnabled;
	Vector3D m_clearColor;

	std::map<std::string, ShaderProgram *> m_shaderPrograms;
	ShaderProgram * m_pCurrentProgram;
	std::string m_debugShaderName;

	bool m_isFullscreen;

	std::string _LoadTextFile(std::string fname);
	bool _GameObjectHasRenderableComponent(const GameObject & gameObject);
	void _RenderFBOSprite(const FBOSprite* pFBOSpriteComp);
	void _RenderSprite(const Sprite* pSpriteComp);
	void _RenderParticles(const ParticleEmitter * pParticleEmitterComp);
	void _RenderText(const Text* pTextComp, const Transform* pTransformComp);
	void _SelectShaderProgram(const Component* renderingComponent);
	void _SetUpCamera(const GameObject& camera);
	void _SetUpLights(const GameObject& gameObject, GameObjectLayer& gol);

	void _SetUpDebug(const GameObject& camera);
	void _RenderDebugCommand(DebugShape shape, const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderRect(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderCircle(const Vector3D & color, float radius, const Vector3D& position);
	void _RenderLine(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& scale);
	void _RenderCone(const Vector3D & color, const Vector3D& pos, const Vector3D& rot, const Vector3D& arcWidthAndRadius);

	void _BindGameObjectTransform(const GameObject& gameObject);
	void _BindGameObjectTransformWithOffset(const GameObject& gameObject, const Vector3D& offset);
	void _BindVertexAttribute(SHADER_LOCATIONS location, GLuint bufferID, unsigned int size, int type, int normalized, int stride = 0, int offset = 0);
	void _BindUniform2(SHADER_LOCATIONS location, const Vector3D& values);
	void _BindUniform2(SHADER_LOCATIONS location, float val1, float val2);
	void _BindUniform3(SHADER_LOCATIONS location, const Vector3D& values);
	void _BindUniform4(SHADER_LOCATIONS location, const Vector3D& values);
public:
	RenderManager(int width = 1200, int height = 800, std::string title = "Default Window Title");
	~RenderManager();
	RenderManager(const RenderManager &) = delete;
	void operator=(const RenderManager &) = delete;

	bool InitGlew();

	Vector3D GetClearColor() const { return m_clearColor; }
	void SetGlobalAmbientLight(Vector3D ambientLight) { m_globalAmbientLight = ambientLight; }
	void FrameStart();
	void FrameEnd();
	void Resize(int width, int height);
	virtual void HandleEvent(Event * p_event);

	void SetUpConsole();
	void InitWindow(bool debugEnabled, bool startFullScreen);
	void EnableWindowsCursor();
	void DisableWindowsCursor(); 
	void SetWindowToFullscreen();
	void UnsetWindowFullscreen();
	void SetWindowWidth(int width);
	void SetWindowHeight(int height);
	void SetWindowDimensions(int width, int height);
	void SetWindowTitle(std::string title);
	inline int WindowWidth() { return m_width; }
	inline int WindowHeight() { return m_height; }
	inline void WindowDimensions(int& width, int& height) { width = m_width; height = m_height; }
	float GetAspectRatio() const;
	inline SDL_Window* GetWindow() { return m_pWindow; }

	void RenderGameObject(const GameObject& camera, const GameObject& go, GameObjectLayer& gol);

	void EnableAlphaTest();
	void EnableDepthTest();
	void BindMesh(const Mesh& mesh);
	void ClearBuffer();
	void ClearBuffer(const Vector3D& color);
	GLuint GenerateStreamingVBO(unsigned int size);
	GLuint GenerateFBO(GLuint& fboID, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type);
	void BindWindowFrameBuffer();
	void DrawSceneFBO();

	template <typename BufferType>
	void BindBufferData(const GLuint& bufferID, BufferType& bufferData, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW); // Buffer orphaning
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, bufferData);
	}

	void LoadShaders(const std::vector<std::string>& shaders);
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