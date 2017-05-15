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
	 * \brief initializes the render by opening the window
	 * \param windowName name of window
	 * \param width width of window
	 * \param height height of window
	 * \param gameLogic callback function to game logic
	 * \pre !windowName.empty()
	 * \pre width > 0
	 * \pre height > 0
	 * \post m_window != nullptr
	 * \return true if successful else false
	 */
	bool init(std::string&& windowName, int width, int height, std::function<void()>&& gameLogic);

	/**
	 * \brief render
	 * \pre m_window != nullptr
	 */
	void render();

	/**
	 * \brief key_callback
	 * \param window
	 * \param key
	 * \param scancode
	 * \param action
	 * \param mode
	 * \pre window != nullptr
	 */
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

private:
	GLFWwindow* m_window;
	int m_width, m_height;
	int m_frameBufferWidth, m_frameBufferHeight;
	std::function<void()> m_gameLogic;
};
