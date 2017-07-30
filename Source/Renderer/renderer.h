#pragma once

#include <functional>
#include <memory>
#include <string>

//GLEW must be included before GLFW
#define GLEW_STATIC
#include <3rdParty\GL\glew.h>
//GLFW
#include <3rdParty\GLFW\glfw3.h>

#include "interfaces.h"
#include "Renderer\shaderprogram.h"

class Renderer : public IRenderer {
public:

	/**
	 * \brief Renderer
	 */
	Renderer();

	/**
	 * \brief Renderer
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
	 * \post m_shaderProgram != nullptr
	 * \post m_shaderProgram->validate()
	 * \post m_window != nullptr
	 * \return true if successful else false
	 */
	bool initialize(std::string&& windowName, int width, int height, std::function<void()>&& gameLogic) override;

	/**
	 * \brief The main Loop. Renders and calls gameLogic onUpdate
	 * \pre m_window != nullptr
	 * \pre m_shaderProgram != nullptr
	 * \pre m_shaderProgram->validate()
	 */
	void startMainLoop() override;

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
	std::unique_ptr<ShaderProgram> m_shaderProgram;

	GLuint m_VBO; // vertex buffer object
	GLuint m_VAO; // vertex array object
	GLuint m_EBO; // vertex element object
	GLfloat m_vertices[12];

	std::function<void()> m_gameLogic;
};
