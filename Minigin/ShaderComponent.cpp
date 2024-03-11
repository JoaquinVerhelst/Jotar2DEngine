#include "ShaderComponent.h"
#include "glad/glad.h"
#include <stdexcept>


dae::ShaderComponent::ShaderComponent(GameObject* owner, const std::string& vertex, const std::string& fragment)
	:Component(owner)
{
	m_ShaderProgram = glCreateProgram();

	// vertex shader 
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* glSourceVertex = vertex.c_str();
	glShaderSource(vertexShaderID, 1, &glSourceVertex, nullptr);
	glCompileShader(vertexShaderID);

	int status = GL_FALSE;

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char errorLog[512];
		glGetShaderInfoLog(vertexShaderID, sizeof(errorLog), nullptr, errorLog);
		glDeleteShader(vertexShaderID);
		throw std::runtime_error(std::string("Failed To Compile Vertex Shader: ") + errorLog);
		return;
	}

	glAttachShader(m_ShaderProgram, vertexShaderID);


	// fragment Shader

	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* glSourceFragment = fragment.c_str();
	glShaderSource(fragmentShaderID, 1, &glSourceFragment, nullptr);
	glCompileShader(fragmentShaderID);

	status = GL_FALSE;

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char errorLog[512];
		glGetShaderInfoLog(fragmentShaderID, sizeof(errorLog), nullptr, errorLog);
		glDeleteShader(fragmentShaderID);
		throw std::runtime_error(std::string("Failed To Compile Fragment Shader: ") + errorLog);
		return;
	}

	glAttachShader(m_ShaderProgram, fragmentShaderID);


	// Shader Program

	glLinkProgram(m_ShaderProgram);
	glValidateProgram(m_ShaderProgram);
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char errorLog[512];
		glGetProgramInfoLog(m_ShaderProgram, sizeof(errorLog), nullptr, errorLog);
		glDeleteProgram(m_ShaderProgram);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		throw std::runtime_error(std::string("Failed To link Shader: ") + errorLog);
		return;
	}


	glDetachShader(m_ShaderProgram, vertexShaderID);
	glDetachShader(m_ShaderProgram, fragmentShaderID);

}

dae::ShaderComponent::~ShaderComponent()
{
	glUseProgram(0);
	glDeleteProgram(m_ShaderProgram);
}

void dae::ShaderComponent::Bind()
{
	glUseProgram(m_ShaderProgram);
}

void dae::ShaderComponent::Unbind()
{
	glUseProgram(0);
}

void dae::ShaderComponent::SetUniformInt(const std::string& name, int val)
{
	glUseProgram(m_ShaderProgram);
	glUniform1i(GetUniformLocation(name), val);
}

void dae::ShaderComponent::SetUniformFloat(const std::string& name, float val)
{
	glUniform1f(GetUniformLocation(name), val);
}

void dae::ShaderComponent::SetUniformFloat2(const std::string& name, float val1, float val2)
{
	glUniform2f(GetUniformLocation(name), val1, val2);
}

void dae::ShaderComponent::SetUniformFloat3(const std::string& name, float val1, float val2, float val3)
{
	glUniform3f(GetUniformLocation(name), val1, val2, val3);
}
void dae::ShaderComponent::SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4)
{
	glUniform4f(GetUniformLocation(name), val1, val2, val3, val4);
}

int dae::ShaderComponent::GetUniformLocation(const std::string& name)
{
	auto it = m_UniformLocations.find(name);
	if (it == m_UniformLocations.end())
	{
		m_UniformLocations[name] = glGetUniformLocation(m_ShaderProgram, name.c_str());
	}

	return m_UniformLocations[name];
}
