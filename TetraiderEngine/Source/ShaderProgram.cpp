/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ShaderProgram::ShaderProgram() :
	m_pVertexShader(NULL),
	m_pFragmentShader(NULL)
{
	m_programID = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	if (m_pVertexShader) {
		glDetachShader(m_programID, m_pVertexShader->GetShaderID());
		delete m_pVertexShader;
	}
	if (m_pFragmentShader) {
		glDetachShader(m_programID, m_pFragmentShader->GetShaderID());
		delete m_pFragmentShader;
	}
}

GLint ShaderProgram::GetProgramID() const
{
	return m_programID;
}

GLint & ShaderProgram::GetProgramIDRef()
{
	return m_programID;
}

bool ShaderProgram::operator==(const ShaderProgram& rhs) const
{
	return rhs.m_programID == m_programID;
}

bool ShaderProgram::operator!=(const ShaderProgram& rhs) const
{
	return rhs.m_programID != m_programID;
}

void ShaderProgram::AttachShader(Shader& shader)
{
	switch (shader.GetShaderType()) {
		case VERTEX_SHADER:
			m_pVertexShader = &shader;
			break;
		case FRAGMENT_SHADER:
			m_pFragmentShader = &shader;
			break;
	}
	glAttachShader(m_programID, shader.GetShaderID());
}

void ShaderProgram::LinkShaders()
{
	GLint Result;
	int InfoLogLength;
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(m_programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}
	if (Result != GL_TRUE)
		std::cout << "LINKING FAILED : " << m_programID << std::endl;
}