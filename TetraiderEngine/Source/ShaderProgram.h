/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ShaderProgram.h
Purpose: Houses a Shader Program for that can have individual shaders linked to it
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glew.h>
#include <map>
#include <string>
#include "Shader.h"

class ShaderProgram
{
protected:
	GLint m_programID;
	Shader * m_pVertexShader;
	Shader * m_pFragmentShader;
	std::map<std::string, GLint> m_attributes;
	std::map<std::string, GLint> m_uniforms;

	GLint _GetAttributeLocation(std::string attribute);
	GLint _GetUniformLocation(std::string uniform);

public:
	ShaderProgram();
	~ShaderProgram();

	GLint GetProgram() const;
	GLint& GetProgramRef();

	void LoadShader(std::string shaderPath, SHADER_TYPE type);

	virtual void AttachShader(Shader& shader);
	void LinkShaders();

	void AddAttribute(std::string name);
	GLint GetAttribute(std::string name);

	void AddUniform(std::string name);
	GLint GetUniform(std::string name);
};

#endif