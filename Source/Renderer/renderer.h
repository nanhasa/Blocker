#pragma once

#include <functional>
#include <memory>
#include <string>

//GLEW must be included before GLFW
#define GLEW_STATIC
#include <3rdParty/GL/glew.h>
//GLFW
#include <3rdParty/GLFW/glfw3.h>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "interfaces.h"
#include "Renderer/shaderprogram.h"

class Renderer : public IRenderer {
public:

	/**
	 * \brief Builder. Does not make valid object as vInitialize() call is needed to make object valid
	 */
	Renderer();

	/**
	 * \brief Destructor
	 */
	~Renderer();

	/**
	 * \brief Initializes the render by opening the window. Must be run before anything else.
	 * \param windowName Name of window
	 * \param width Width of window
	 * \param height Height of window
	 * \param gameLogic Callback function used to update game logic on every tick
	 * \pre width >= 0
	 * \pre height >= 0
	 * \post m_shaderProgram != nullptr
	 * \post m_shaderProgram->validate()
	 * \post m_window != nullptr
	 * \return true if successful, otherwise false
	 */
	bool vInitialize(std::string&& windowName, int width, int height,
		std::function<void(float)>&& gameLogic) override;

	/**
	 * \brief The main Loop. Renders, gets device input and calls gameLogic onUpdate
	 * \pre m_window != nullptr
	 * \pre m_shaderProgram != nullptr
	 * \pre m_shaderProgram->validate()
	 */
	void vStartMainLoop() override;

	/**
	 * \brief Callback for key input
	 * \param window Window that received input
	 * \param key Key pressed
	 * \param scancode ??
	 * \param action ??
	 * \param mode Mode of key input
	 * \pre window != nullptr
	 */
	static void staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	/**
	 * \brief Member key input function called by staticKeyCallback
	 * \param key Key pressed
	 * \param scancode ??
	 * \param action ??
	 * \param mode Mode of key input
	 * \pre m_window != nullptr
	 */
	void keyCallback(int key, int scancode, int action, int mode);

	//static void staticMouseCallback(GLFWwindow* window, double xpos, double ypos);

	//void mouseCallback(double xpos, double ypos);

	/**
	 * \brief Callback for mouse events and movement
	 * \param window Window that received input
	 * \param width New frame buffer width
	 * \param height New frame buffer height
	 * \pre window != nullptr
	 * \pre width >= 0
	 * \pre height >= 0
	 */
	static void staticFramebufferSizeCallback(GLFWwindow* window, int width, int height);

	/**
	 * \brief Member framebuffer size callback function called by staticFramebufferSizeCallback
	 * \param width New frame buffer width
	 * \param height New frame buffer height
	 * \pre m_window != nullptr
	 * \post framebufferWidth == width
	 * \post framebufferHeight == height
	 */
	void framebufferSizeCallback(int width, int height) const;

	/**
	 * \brief vSetViewMatrice
	 * \param viewMatrix
	 */
	void vSetViewMatrix(const glm::mat4& viewMatrix) override;


	/**
	 * \brief vGetCursorPosition
	 * \param x
	 * \param y
	 * \pre m_window
	 */
	void vGetCursorPosition(double& x, double& y) const override;


	/**
	 * \brief vCenterCursor
	 * \pre m_window
	 */
	void vCenterCursor() override;

	/**
	 * \brief vGetKeyInput
	 * \param key
	 * \pre m_window
	 * \return
	 */
	bool vKeyPressed(int key) const override;

	/**
	 * \brief vGetPhysicalScreenResolution
	 * \param x
	 * \param y
	 */
	void vGetPhysicalScreenResolution(int& x, int& y) const override;

private:
	GLFWwindow* m_window;			//!< Pointer to GLFW window object
	int m_screenResolutionWidth;	//!< Physical screen resolution width
	int m_screenResolutionHeight;	//!< Physical screen resolution height

	std::unique_ptr<ShaderProgram> m_shaderProgram;	//!< Shader program used to access shaders

	GLuint m_VBO; //!< vertex buffer object
	GLuint m_VAO; //!< vertex array object
	GLuint m_EBO; //!< vertex element object

	glm::mat4 m_model;		//!< Matrice From local space to world space
	glm::mat4 m_view;		//!< Matcice From world space to view space
	glm::mat4 m_projection;	//!< Matrice From view space to clip space

	Logger m_log;

	std::function<void(float)> m_gameLogic; //!< Function object used to update game logic
};
