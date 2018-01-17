#include "ShaderProgram.h"
#include <iostream>
#include <vector>

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

GLint ShaderProgram::GetProgram() const
{
	return m_programID;
}

GLint & ShaderProgram::GetProgramRef()
{
	return m_programID;
}

void ShaderProgram::LoadShader(std::string shaderPath, SHADER_TYPE type)
{
	
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
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(m_programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}
}

GLint ShaderProgram::_GetAttributeLocation(std::string attribute)
{
	return glGetAttribLocation(m_programID, attribute.c_str());
}

GLint ShaderProgram::_GetUniformLocation(std::string uniform)
{
	return glGetUniformLocation(m_programID, uniform.c_str());
}

void ShaderProgram::AddAttribute(std::string name)
{
	m_attributes[name] = _GetAttributeLocation(name);
}

GLint ShaderProgram::GetAttribute(std::string name)
{
	return m_attributes[name];
}

void ShaderProgram::AddUniform(std::string name)
{
	m_uniforms[name] = _GetUniformLocation(name);
}

GLint ShaderProgram::GetUniform(std::string name)
{
	return m_uniforms[name];
}
