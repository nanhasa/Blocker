#include "Renderer/shaderprogram.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/gtc/type_ptr.hpp>
#pragma warning (pop)      // Restore back

#include "Utility/contract.h"
#include "Utility/config.h"
#include "Utility/locator.h"
#include "Utility/utility.h"

ShaderProgram::ShaderProgram() : m_id(glCreateProgram()), m_log("ShaderProgram") {}

ShaderProgram::~ShaderProgram() { glDeleteProgram(m_id); }

GLuint ShaderProgram::getID() const { return m_id; }

bool ShaderProgram::attachShader(const std::string& filename, GLenum shaderType) const
{
	REQUIRE(!filename.empty());

	m_log.info("Started attaching shader");

	// Create shader
	const GLuint shader = glCreateShader(shaderType);

	// Load shader file
	std::string sourcestr{};
	if (!loadShader(filename, sourcestr))
		return false;
	const char* sourcechar = sourcestr.c_str();

	glShaderSource(shader, 1, &sourcechar, NULL);
	glCompileShader(shader);
	if (!validateShaderObject(shader, GL_COMPILE_STATUS)) {
		glDeleteShader(shader);
		return false;
	}
	m_log.info("Shader compilation successful");

	// Attach shader to shader program and delete shader afterwards
	glAttachShader(m_id, shader);
	glDeleteShader(shader);

	m_log.info(filename + " successfully attached to shaderprogram " + utility::toStr(m_id));

	glLinkProgram(m_id);

	// TODO: Ensure that there is one more shader than at the beginning

	return validateShaderObject(m_id, GL_LINK_STATUS); // Validate program linking
}

bool ShaderProgram::validate() const
{
	if (!glIsProgram(m_id)) {
		m_log.error("Shader program not valid");
		return false;
	}
	return validateShaderObject(m_id, GL_LINK_STATUS);
}

void ShaderProgram::use() const { glUseProgram(m_id); }

void ShaderProgram::setBool(const std::string& name, bool value) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetBool");
		return;
	}

	setInt(name, static_cast<int>(value));
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetInt");
		return;
	}

	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetFloat");
		return;
	}

	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetVec2");
		return;
	}

	glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec2(const std::string& name, float x, float y) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetVec2");
		return;
	}

	glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetVec3");
		return;
	}

	glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetVec3");
		return;
	}

	glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetVec4");
		return;
	}

	glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetVec4");
		return;
	}

	glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
}

void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetMat2");
		return;
	}

	glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetMat3");
		return;
	}

	glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in SetMat4");
		return;
	}

	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

bool ShaderProgram::loadShader(const std::string& name, std::string& shaderSource) const
{
	REQUIRE(!name.empty());
	if (name.empty()) {
		m_log.error("Invalid name in loadShader");
		return false;
	}

	m_log.info("Loading shader file: " + name);

	if (!shaderSource.empty())
		shaderSource.clear();

	std::ifstream file(Locator::getConfig()->get("DataPath", std::string("../Data/")) + "Shaders/" + name);
	if (!file.is_open()) {
		m_log.error("Could not open shader: " + name);
		return false;
	}

	std::stringstream shaderData;
	shaderData << file.rdbuf(); // Loads the entire string into a stringstream.

	// Get file length
	const std::streamoff filesize = file.tellg();
	file.close();

	// Get contents to string and count new lines in it to compare it to file length
	shaderSource = std::move(shaderData.str());
	const unsigned int newLines = static_cast<unsigned int>(std::count(shaderSource.begin(), shaderSource.end(), '\n'));

	ENSURE(!shaderSource.empty());
	ENSURE(shaderSource.size() + newLines == static_cast<unsigned int>(filesize));

	if (shaderSource.empty()) {
		m_log.error("Empty shader file : " + name);
		return false;
	}
	if (shaderSource.size() + newLines != static_cast<unsigned int>(filesize)) {
		m_log.error("Shader loaded does not match shader file: " + name);
		return false;
	}
	m_log.info("Shader file loaded successfully");

	return true;
}

bool ShaderProgram::validateShaderObject(GLuint object, GLenum paramType) const
{
	REQUIRE(object != 0);
	if (object == 0) {
		m_log.error("Shader object not valid");
		return false;
	}

	GLint success;
	GLchar infoLog[1024];
	if (paramType != GL_LINK_STATUS) {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			m_log.error("SHADER_COMPILATION_ERROR: " + std::string(infoLog));
			return false;
		}
		return true;
	}

	glGetProgramiv(object, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(object, 1024, NULL, infoLog);
		m_log.error("ERROR::PROGRAM_LINKING_ERROR: " + std::string(infoLog));
		return false;
	}
	return true;
}
