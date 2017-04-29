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
	Renderer();
	~Renderer();

	bool init(std::string&& windowName, int width, int height, std::function<void()>&& gameLogic);
	void render();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

private:
	GLFWwindow* m_window;
	int m_width, m_height;
	int m_frameBufferWidth, m_frameBufferHeight;
	std::function<void()> m_gameLogic;
};