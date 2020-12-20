#include "shader.hpp"
#include "../logger.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include <fstream>

DG::Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	//Reads the shader sources from files
	std::string vertexSource;
	std::string fragmentSource;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		std::stringstream vertexStream;
		std::stringstream fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexSource = vertexStream.str();
		fragmentSource = fragmentStream.str();
	}
	catch (std::ifstream::failure e)
	{
		//Can't open the file for some reason
		DG::log << "Shader retrival error: " << e.code() << "\n";
		DG::dumpLog();
		exit(-4);
	}

	//Builds and links the shaders to the program
	const char* cVertexSource = vertexSource.c_str();
	const char* cFragmentSource = fragmentSource.c_str();

	unsigned int vertex;
	unsigned int fragment;

	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &cVertexSource, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		DG::log << "Error compiling vertex shader: " << infoLog << "\n";
		DG::dumpLog();
		exit(-5);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &cFragmentSource, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		DG::log << "Error compiling fragment shader: " << infoLog << "\n";
		DG::dumpLog();
		exit(-5);
	}

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		DG::log << "Error compiling shader program: " << infoLog << "\n";
		DG::dumpLog();
		exit(-5);
	}

	//Deletes shaders after they're compiled
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void DG::Shader::use()
{
	glUseProgram(id);
}

void DG::Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void DG::Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void DG::Shader::setFloat(const std::string& name, float value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void DG::Shader::setMat4(const std::string& name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}