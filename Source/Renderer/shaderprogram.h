#pragma once

#include <string>

//GLEW must be included before GLFW
#define GLEW_STATIC
#include <3rdParty\GL\glew.h>
//GLFW
#include <3rdParty\GLFW\glfw3.h>

class ShaderProgram {
public:

	/**
	 * \brief Builder. Call attachShader to initialize object.
	 */
	ShaderProgram();

	/**
	 * \brief Destructor
	 */
	~ShaderProgram();

	/**
	 * \brief Reads shader source file, compiles it, attaches, and links it to shaderProgram
	 * \param filename Name of the shader source file, default path is Game/Data/Shaders/
	 * \param shaderType GLenum that identifies which type of shader is loaded
	 * \pre !filename.empty()
	 * \pre shaderType != NULL
	 * \return true if everything was successful
	 */
	bool attachShader(const std::string& filename, GLenum shaderType) const;

	/**
	 * \brief Validates shader program linking and is it recognized by glfw
	 * \return true if valid
	 */
	bool validate() const;

	/**
	 * \brief Set shader program active
	 */
	void use() const;

	/**
	 * \brief Utility bool uniform
	 * \param name Uniform name
	 * \param value Uniform value to be updated
	 */
	void setBool(const std::string &name, bool value) const;

	/**
	 * \brief Utility int uniform
	 * \param name Uniform name
	 * \param value Uniform value to updated
	 */
	void setInt(const std::string &name, int value) const;

	/**
	 * \brief Utility float uniform
	 * \param name Uniform name
	 * \param value Uniform value to be updated
	 */
	void setFloat(const std::string &name, float value) const;

private:
	GLuint m_id;

	/**
	 * \brief Loads shader file into shaderSource parameter string
	 * \param name Shader file name to be loaded without file path
	 * \param shaderSource Output variable that stores contents of file loaded
	 * \pre !name.empty()
	 * \post !shaderSource.empty()
	 * \post shaderSource.size() + newLines == static_cast<unsigned int>(filesize)
	 * \return True if successful, otherwise false
	 */
	bool loadShader(const std::string& name, std::string& shaderSource) const;

	/**
	 * \brief Loads shader source file 'name' to shaderSource
	 * \param object Object being validated
	 * \param paramType Aspect being validated
	 * \return true if loading was successful
	 */
	bool validateShaderObject(GLuint object, GLenum paramType) const;
};
