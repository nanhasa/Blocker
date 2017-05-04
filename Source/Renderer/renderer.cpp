#include "renderer.h"
#include "..\contract.h"

#include <iostream>

Renderer::Renderer()
{
	m_window = nullptr;
}

Renderer::~Renderer()
{
	glfwTerminate();
}

bool Renderer::init(std::string && windowName, int width, int height, std::function<void()>&& gameLogic)
{
	REQUIRE(!windowName.empty());
	REQUIRE(width > 0);
	REQUIRE(height > 0);

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	m_width = width;
	m_height = height;
	m_gameLogic = gameLogic;

	// Init GLFW and set the required options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object to for GLFW's functions
	// TODO - read the size from config file
	m_window = glfwCreateWindow(m_width, m_height, std::move(windowName.c_str()), nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE; // Uses more modern techniques for managing OpenGL functionality
	if (glewInit() != GLEW_OK) // Initialize GLEW to setup the OpenGL Function pointers
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	glfwGetFramebufferSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);

	glViewport(0, 0, m_frameBufferWidth, m_frameBufferHeight);

	glfwSetKeyCallback(m_window, Renderer::key_callback);

	ENSURE(m_window != nullptr);

	return true;
}

void Renderer::render()
{
	REQUIRE(m_window != nullptr);

	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		m_gameLogic();

		//Clear color and set it to greenish
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_window);
	}
}

void Renderer::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	REQUIRE(window != nullptr);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Unused variables
	(void)mode;
	(void)scancode;
}
