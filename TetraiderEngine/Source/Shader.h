/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: An invidual vertex or fragment/pixel shader. Can be linked to a ShaderProgram
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include <string>

const enum SHADER_TYPE {
	VERTEX_SHADER = 0,
	FRAGMENT_SHADER = 1
};

class Shader
{
private:
	GLuint m_shaderID;
	SHADER_TYPE m_type;

public:
	Shader();
	Shader(std::string shaderText, SHADER_TYPE type);
	~Shader();

	GLuint GetShaderID() const;
	SHADER_TYPE GetShaderType() const;

	void LoadShaderFromFile(std::string shaderFilePath, SHADER_TYPE type);
};

#endif