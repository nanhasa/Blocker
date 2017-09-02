#pragma once

#include <string>

//GLEW must be included before GLFW
#define GLEW_STATIC
#include <3rdParty/GL/glew.h>
//GLFW

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
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

	/**
	 * \brief Used to get Shader program ID
	 * \return Shader program ID
	 */
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
	 * \brief Set value to bool uniform
	 * \param name Uniform name
	 * \param value Uniform value to be updated
	 * \pre !name.empty()
	 */
	void setBool(const std::string &name, bool value) const;

	/**
	 * \brief Set value to int uniform
	 * \param name Uniform name
	 * \param value Uniform value to updated
	 * \pre !name.empty()
	 */
	void setInt(const std::string &name, int value) const;

	/**
	 * \brief Set value to float uniform
	 * \param name Uniform name
	 * \param value Uniform value to be updated
	 * \pre !name.empty()
	 */
	void setFloat(const std::string &name, float value) const;

	/**
	 * \brief Set value to Vec2 uniform
	 * \param name Uniform name
	 * \param value Vector2 updated
	 * \pre !name.empty()
	 */
	void setVec2(const std::string &name, const glm::vec2 &value) const;

	/**
	 * \brief Set value to Vec2 uniform
	 * \param name Uniform name
	 * \param x Vector x value
	 * \param y Vector y value
	 * \pre !name.empty()
	 */
	void setVec2(const std::string &name, float x, float y) const;

	/**
	 * \brief Set value to Vec3 uniform
	 * \param name Uniform name
	 * \param value Vector3 updated
	 * \pre !name.empty()
	 */
	void setVec3(const std::string &name, const glm::vec3 &value) const;

	/**
	 * \brief Set value to Vec3 uniform
	 * \param name Uniform name
	 * \param x Vector x value
	 * \param y Vector y value
	 * \param z Vector z value
	 * \pre !name.empty()
	 */
	void setVec3(const std::string &name, float x, float y, float z) const;

	/**
	 * \brief Set value to Vec4 uniform
	 * \param name Uniform name
	 * \param value Vector4 updated
	 * \pre !name.empty()
	 */
	void setVec4(const std::string &name, const glm::vec4 &value) const;

	/**
	 * \brief Set value to Vec4 uniform
	 * \param name Uniform name
	 * \param x Vector x value
	 * \param y Vector y value
	 * \param z Vector z value
	 * \param w Vector w value
	 * \pre !name.empty()
	 */
	void setVec4(const std::string &name, float x, float y, float z, float w) const;

	/**
	 * \brief Set value to Mat2 uniform
	 * \param name Uniform name
	 * \param mat Mat2 updated
	 * \pre !name.empty()
	 */
	void setMat2(const std::string &name, const glm::mat2 &mat) const;

	/**
	 * \brief Set value to Mat3 uniform
	 * \param name Uniform name
	 * \param mat Mat3 updated
	 * \pre !name.empty()
	 */
	void setMat3(const std::string &name, const glm::mat3 &mat) const;

	/**
	 * \brief Set value to Mat4 uniform
	 * \param name Uniform name
	 * \param mat Mat4 updated
	 * \pre !name.empty()
	 */
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
	GLuint m_id; //!< Shader program ID

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
	 * \pre object != 0
	 * \return true if loading was successful, otherwise false
	 */
	static bool validateShaderObject(GLuint object, GLenum paramType);
};
