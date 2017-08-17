#pragma once

#include <string>

//GLEW must be included before GLFW
#define GLEW_STATIC
#include <3rdParty/GL/glew.h>
//GLFW
#include <3rdParty/GLFW/glfw3.h>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#include <3rdParty/glm/gtc/type_ptr.hpp>
#pragma warning (pop)      // Restore back


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

	GLuint getID() const;

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

	void setVec2(const std::string &name, const glm::vec2 &value) const;

	void setVec2(const std::string &name, float x, float y) const;

	void setVec3(const std::string &name, const glm::vec3 &value) const;

	void setVec3(const std::string &name, float x, float y, float z) const;

	void setVec4(const std::string &name, const glm::vec4 &value) const;

	void setVec4(const std::string &name, float x, float y, float z, float w) const;

	void setMat2(const std::string &name, const glm::mat2 &mat) const;

	void setMat3(const std::string &name, const glm::mat3 &mat) const;

	void setMat4(const std::string &name, const glm::mat4 &mat) const;

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
