#pragma once

#include <string>
#include <functional>

//GLEW must be included before GLFW
#define GLEW_STATIC
#include <3rdParty\GL\glew.h>
//GLFW
#include <3rdParty\GLFW\glfw3.h>

class Renderer {
public:

	/**
	 * \brief Renderer
	 */
	Renderer();

	/**
	 * \brief ~Renderer
	 */
	~Renderer();

	/**
	 * \brief Initializes the render by opening the window. Must be run before anything else.
	 * \param windowName name of window
	 * \param width width of window
	 * \param height height of window
	 * \param gameLogic callback function to game logic
	 * \pre !windowName.empty()
	 * \pre width >= 0
	 * \pre height >= 0
	 * \post glIsProgram(m_shaderProgram)
	 * \post validateShaderObject(m_shaderProgram, GL_LINK_STATUS)
	 * \post m_window != nullptr
	 * \return true if successful else false
	 */
	bool init(std::string&& windowName, int width, int height, std::function<void()>&& gameLogic);

	/**
	 * \brief The main Loop. Renders and calls gameLogic onUpdate
	 * \pre m_window != nullptr
	 * \pre glIsProgram(m_shaderProgram)
	 * \pre validateShaderObject(m_shaderProgram, GL_LINK_STATUS)
	 */
	void render();

	/**
	 * \brief Callback for key input
	 * \param window Window that received input
	 * \param key Key pressed
	 * \param scancode ??
	 * \param action ??
	 * \param mode Mode of key input
	 * \pre window != nullptr
	 */
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	/**
	 * \brief Callback to adjust frame buffer size
	 * \param window Window that was resized
	 * \param width New frame buffer width
	 * \param height New frame buffer height
	 * \pre width >= 0
	 * \pre height >= 0
	 * \post framebufferWidth == width
	 * \post framebufferHeight == height
	 */
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_window;
	GLuint m_shaderProgram;

	GLuint m_VBO; // vertex buffer object
	GLuint m_VAO; // vertex array object
	GLuint m_EBO; // vertex element object
	GLfloat m_vertices[12];

	std::function<void()> m_gameLogic;

	/**
	 * \brief Reads shader source file, compiles it, and attaches it to shaderProgram
	 * \param shaderProgram shader program to attach the shader
	 * \param filename shader source filename
	 * \param shaderType shader type to be attached
	 * \pre glIsProgram(shaderProgram)
	 * \pre !filename.empty()
	 * \pre shaderType != NULL
	 * \return operation successful
	 */
	bool attachShader(GLuint shaderProgram, std::string filename, GLenum shaderType) const;

	/**
	 * \brief Loads shader source file 'name' to shaderSource
	 * \param name Shader source filename
	 * \param shaderSource Reference parameter to hold the contents of shader source file
	 * \pre !name.empty()
	 * \post !shaderSource.empty()
	 * \return operation successful
	 */
	bool loadShader(std::string name, std::string& shaderSource) const;

	/**
	 * \brief Validates shader object
	 * \param object Shader object to be validated
	 * \param paramType Status to be validated
	 * \pre object != 0
	 * \return validation successful
	 */
	bool validateShaderObject(GLuint object, GLenum paramType) const;
};
