#pragma once

#include <string>

//GLEW must be included before GLFW
#define GLEW_STATIC
#include <3rdParty\GL\glew.h>
//GLFW
#include <3rdParty\GLFW\glfw3.h>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();
	/**
	 * \brief Reads shader source file, compiles it, and attaches it to shaderProgram
	 */
	bool attachShader(const std::string& filename, GLenum shaderType) const;
	bool validate() const;

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

private:
	GLuint m_id;

	/**
	 * \brief Loads shader source file 'name' to shaderSource
	 */
	bool loadShader(const std::string& name, std::string& shaderSource) const;
	/**
	 * \brief Validates shader object
	 */
	bool validateShaderObject(GLuint object, GLenum paramType) const;
};
