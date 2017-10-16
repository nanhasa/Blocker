#include "Renderer/renderer.h"

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#pragma warning (pop)      // Restore back

#include "Event/eventmanager.h"
#include "Renderer/fileloader.h"
#include "Utility/contract.h"
#include "Utility/locator.h"
#include "Utility/utility.h"

Renderer::Renderer()
	: m_window(nullptr), m_width(0), m_height(0), m_sizeChanged(false), 
	m_shaderProgram(nullptr), m_VBO(0), m_VAO(0), m_EBO(0), m_log("Renderer") {}

Renderer::~Renderer()
{
	glfwTerminate();
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

bool Renderer::vInitialize(std::string&& windowName, std::function<void(float)>&& gameLogic)
{
	m_log.info("Starting GLFW context, OpenGL 3.3");

	m_gameLogic = gameLogic;

	// Init GLFW and set the required options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object to for GLFW's functions
	const int width = Locator::getConfig()->get("ScreenWidth", 1600);
	const int height = Locator::getConfig()->get("ScreenHeight", 1200);
	m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	
	if (m_window == nullptr) {
		m_log.fatal("Failed to create GLFW window");
		return false;
	}
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE; // Uses more modern techniques for managing OpenGL functionality
	if (glewInit() != GLEW_OK) {
		// Initialize GLEW to setup the OpenGL Function pointers
		m_log.fatal("Failed to initialize GLEW");
		return false;
	}

	glfwGetFramebufferSize(m_window, &m_width, &m_height);
	glViewport(0, 0, m_width, m_height);

	// Set callback functions to static functions
	glfwSetFramebufferSizeCallback(m_window, Renderer::staticFramebufferSizeCallback); // Resize
	glfwSetKeyCallback(m_window, Renderer::staticKeyCallback); // Key

	// Cannot bind member functions to callbacks so bind window with pointer to this
	// With this we can call member functions from static callback functions
	glfwSetWindowUserPointer(m_window, this);

	// Do not let cursor out of the screen
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST); // Enable depth testing

	// Create shader program by attaching and linking shaders to it
	m_shaderProgram = std::make_unique<ShaderProgram>();
	if (!m_shaderProgram->attachShader("vertex_basic.vert", GL_VERTEX_SHADER)) {
		m_log.fatal("Could not attach shader: vertex_basic.vert");
		return false;
	}
	if (!m_shaderProgram->attachShader("fragment_basic.frag", GL_FRAGMENT_SHADER)) {
		m_log.fatal("Could not attach shader: fragment_basic.frag");
		return false;
	}

	float vertices[] = {
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		0.0f,
		0.5f,
		-0.5f,
		-0.5f,
		1.0f,
		0.0f,
		0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f,
		0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f,
		-0.5f,
		0.5f,
		-0.5f,
		0.0f,
		1.0f,
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		0.0f,

		-0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f,
		0.5f,
		-0.5f,
		0.5f,
		1.0f,
		0.0f,
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		1.0f,
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		1.0f,
		-0.5f,
		0.5f,
		0.5f,
		0.0f,
		1.0f,
		-0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f,

		-0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f,
		-0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f,
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f,
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f,
		-0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f,
		-0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f,

		0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f,
		0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f,
		0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f,
		0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f,
		0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f,
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f,

		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f,
		0.5f,
		-0.5f,
		-0.5f,
		1.0f,
		1.0f,
		0.5f,
		-0.5f,
		0.5f,
		1.0f,
		0.0f,
		0.5f,
		-0.5f,
		0.5f,
		1.0f,
		0.0f,
		-0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f,
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f,

		-0.5f,
		0.5f,
		-0.5f,
		0.0f,
		1.0f,
		0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f,
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f,
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f,
		-0.5f,
		0.5f,
		0.5f,
		0.0f,
		0.0f,
		-0.5f,
		0.5f,
		-0.5f,
		0.0f,
		1.0f
	};

	unsigned int indices[] = {
		// note that we start from 0!
		0,
		1,
		3,
		// first Triangle
		1,
		2,
		3 // second Triangle
	};

	// Generate vertex management objects
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	// Bind vertex array object
	glBindVertexArray(m_VAO);
	// Bind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Copy vertices array to a vertex buffer object for OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Bind element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// Initialize member matrices
	m_model = glm::rotate(glm::mat4(), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// Translate scene in the reverse direction of where we want to move
	m_projection = glm::perspective(glm::radians(45.0f),
	                                static_cast<float>(width / height), 0.1f, 100.0f);

	// Unbind vertex objects now that they are registered so that they aren't misconfigured again by accident
	// 0 resets the currently bound buffer to a NULL like state
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
	glBindVertexArray(0); // Unbind VAO

	ENSURE(m_shaderProgram != nullptr);
	ENSURE(m_shaderProgram->validate());
	ENSURE(m_window != nullptr);
	m_log.info("OpenGL initialized succesfully");

	return true;
}

void Renderer::vStartMainLoop()
{
	REQUIRE(m_window != nullptr);
	REQUIRE(m_shaderProgram != nullptr);
	REQUIRE(m_shaderProgram->validate());
	if (m_window == nullptr || m_shaderProgram == nullptr || !m_shaderProgram->validate()) {
		m_log.error("OpenGL not properly initialized before calling vStartMainLoop");
		return;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set the fileloader wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set fileloader filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load fileloader data
	std::unique_ptr<fileloader::Image> txrData = fileloader::loadTexture("square.bmp");
	const unsigned int width = txrData->getWidth();
	const unsigned int height = txrData->getHeight();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, txrData->getData());
	glGenerateMipmap(GL_TEXTURE_2D);
	txrData.reset();

	// Test values, later let gamemanager push these here
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	// Center cursor now that we are ready to start the game
	vCenterCursor();

	m_log.info("Started main loop");
	int previousTick = utility::timestampMs();
	while (!glfwWindowShouldClose(m_window)) {
		const int currentTick = utility::timestampMs();
		float const deltatime = static_cast<float>(currentTick - previousTick) / 1000;

		glfwPollEvents();

		m_gameLogic(deltatime);

		// Clear depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Clear color and set it to greenish
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind Texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		m_shaderProgram->use();

		// Set uniforms
		m_shaderProgram->setMat4("view", m_view);
		m_shaderProgram->setMat4("projection", m_projection);

		// Render container
		glBindVertexArray(m_VAO);
		for (const auto cube : cubePositions) {
			m_model = glm::translate(glm::mat4(), cube);
			m_shaderProgram->setMat4("model", m_model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0); // Unbind
		glfwSwapBuffers(m_window);

		m_sizeChanged = false; // Reset sizeChanged variable

		previousTick = currentTick;
	}
	m_log.info("Leaving from main loop");
}

void Renderer::staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	REQUIRE(window != nullptr);
	if (window == nullptr) {
		return;
	}

	Renderer* r = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	r->keyCallback(key, scancode, action, mode);
}

void Renderer::keyCallback(int key, int scancode, int action, int mode)
{
	REQUIRE(m_window != nullptr);
	if (m_window == nullptr) {
		m_log.error("OpenGL not properly initialized before calling keyCallback");
		return;
	}

	// Unused variables
	(void)mode;
	(void)scancode;

	// Handle general input
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, GL_TRUE); // Close window on ESC
		return;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode on M
		return;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Normal mode on N
		return;
	}
}

void Renderer::staticFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	REQUIRE(window != nullptr);
	REQUIRE(width >= 0);
	REQUIRE(height >= 0);
	if (window == nullptr) {
		return;
	}
	if (width < 0 || height < 0) {
		return;
	}

	Renderer* r = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	r->framebufferSizeCallback(width, height);
}

void Renderer::framebufferSizeCallback(int width, int height)
{
	REQUIRE(m_window != nullptr);
	if (m_window == nullptr) {
		m_log.error("OpenGL not properly initialized before calling framebufferSizeCallback");
		return;
	}

	// Adjust viewport for window changes
	glViewport(0, 0, width, height);
	glfwGetFramebufferSize(m_window, &m_width, &m_height);
	m_sizeChanged = true;

	ENSURE(m_width == width);
	ENSURE(m_height == height);
}

void Renderer::vSetViewMatrix(const glm::mat4& viewMatrix)
{
	m_view = viewMatrix;
}

void Renderer::vGetCursorPosition(double& x, double& y) const
{
	REQUIRE(m_window);
	if (!m_window) {
		m_log.error("OpenGL not properly initialized before calling vGetMousePosition");
		return;
	}
	glfwGetCursorPos(m_window, &x, &y);
}

void Renderer::vCenterCursor() const
{
	REQUIRE(m_window);
	if (!m_window) {
		m_log.error("OpenGL not properly initialized before calling vSetMousePosition");
		return;
	}
	glfwSetCursorPos(m_window, m_width / 2, m_height / 2);
}

bool Renderer::vKeyPressed(int key) const
{
	REQUIRE(m_window);
	if (!m_window) {
		m_log.error("OpenGL not properly initialized before calling vGetKeyState");
		return false;
	}
	return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool Renderer::vWindowSizeChanged() const
{
	return m_sizeChanged;
}
