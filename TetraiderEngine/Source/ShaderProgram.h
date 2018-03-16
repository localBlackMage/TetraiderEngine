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

//#include <glew.h>
//#include <map>
//#include <string>
//#include "Shader.h"

class ShaderProgram
{
protected:
	GLint m_programID;
	Shader * m_pVertexShader;
	Shader * m_pFragmentShader;

public:
	ShaderProgram();
	~ShaderProgram();

	GLint GetProgramID() const;
	GLint& GetProgramIDRef();

	bool operator==(const ShaderProgram& rhs) const;
	bool operator!=(const ShaderProgram& rhs) const;

	virtual void AttachShader(Shader& shader);
	void LinkShaders();
};

#endif