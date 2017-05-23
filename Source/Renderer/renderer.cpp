#include "renderer.h"
#include "..\contract.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Renderer::Renderer()
{
	m_window = nullptr;
}

Renderer::~Renderer()
{
	glfwTerminate();
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
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
	glfwSetKeyCallback(m_window, Renderer::key_callback);

	glViewport(0, 0, m_frameBufferWidth, m_frameBufferHeight);

	// Create shader program by attaching and linking shaders to it
	m_shaderProgram = glCreateProgram();
	if (!attachShader(m_shaderProgram, "vertex_basic.vert", GL_VERTEX_SHADER))
		return false;
	if (!attachShader(m_shaderProgram, "fragment_basic.frag", GL_FRAGMENT_SHADER))
		return false;
	glLinkProgram(m_shaderProgram); // Link attached shaders to shaderProgram


	// Triangle vertices: left, right, top
	GLfloat triangle[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	std::copy(triangle, triangle + 9, m_vertices);

	// Generate vertex management objects
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	// Bind vertex array object
	glBindVertexArray(m_VAO);
	// Bind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Copy vertices array to a vertex buffer object for OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind vertex objects now that they are registered so that they aren't misconfigured again by accident
	// 0 resets the currently bound buffer to a NULL like state
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
	glBindVertexArray(0); // Unbind VAO

	ENSURE(m_shaderProgram != 0);
	ENSURE(validateShaderObject(m_shaderProgram, GL_LINK_STATUS));
	ENSURE(m_window != nullptr);
	std::cout << "OpenGL initialized succesfully" << std::endl;

	return true;
}

void Renderer::render()
{
	REQUIRE(m_window != nullptr);
	REQUIRE(m_shaderProgram != 0);
	REQUIRE(validateShaderObject(m_shaderProgram, GL_LINK_STATUS));

	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		m_gameLogic();

		// Clear color and set it to greenish
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_shaderProgram);
		glBindVertexArray(m_VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(m_window);
	}
}

void Renderer::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	REQUIRE(window != nullptr);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Unused variables
	(void)mode;
	(void)scancode;
}

bool Renderer::attachShader(GLuint shaderProgram, std::string filename, GLenum shaderType) const 
{
	REQUIRE(shaderProgram != 0);
	REQUIRE(!filename.empty());
	REQUIRE(shaderType != NULL);

	// Load shader file
	std::string sourcestr{};
	if (!loadShader(filename, sourcestr))
		return false;
	const char* shaderSource = sourcestr.c_str();

	// Compile shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	if (!validateShaderObject(shader, GL_COMPILE_STATUS))
	{
		std::cout << "Shader compilation failed" << std::endl;
		glDeleteShader(shader);
		return false;
	}
	std::cout << "Shader compilation successful" << std::endl;

	// Attach shader to shader program and delete shader afterwards
	glAttachShader(shaderProgram, shader);
	glDeleteShader(shader);

	std::cout << filename + " successfully attached to shaderprogram" << std::endl;

	return true;
}

bool Renderer::loadShader(std::string name, std::string& shaderSource) const
{
	REQUIRE(!name.empty());
	
	if (!shaderSource.empty())
		shaderSource.clear();

	std::ifstream file("../Data/Shaders/" + name);
	if (!file.is_open())
	{
		std::cout << "Could not open shader: " + name << std::endl;
		return false;
	}

	std::stringstream shaderData;
	shaderData << file.rdbuf();  // Loads the entire string into a string stream.
	file.close();
	shaderSource = std::move(shaderData.str());

	if (shaderSource.empty()) 
	{
		std::cout << "Empty shader file: " + name << std::endl;
		return false;
	}

	ENSURE(!shaderSource.empty());
	return true;
}

bool Renderer::validateShaderObject(GLuint object, GLenum paramType) const
{
	REQUIRE(object != 0);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(object, paramType, &success);
	if (!success)
	{
		switch (paramType) 
		{
		case GL_COMPILE_STATUS:
			glGetShaderInfoLog(object, 512, NULL, infoLog);
			std::cout << "Shader compilation failed:\n" << infoLog << std::endl;
			break;

		case GL_LINK_STATUS:
			glGetProgramInfoLog(object, 512, NULL, infoLog);
			std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
			break;

		default:
			std::cout << "Undefined paramType in validateShaderObject()" << std::endl;
			return false;
		}	
		return false;
	}
	return true;
}
