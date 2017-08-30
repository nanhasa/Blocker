#include <iostream>

#include "contract.h"
#include "Event/eventmanager.h"
#include "Event/inputcommandevent.h"
#include "Renderer/renderer.h"
#include "Renderer/texture.h"
#include "utility.h"

Renderer::Renderer()
	: m_window(nullptr), m_shaderProgram(nullptr),
	  m_VBO(0), m_VAO(0), m_EBO(0), m_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
	  m_firstMouseMovement(true), m_mousexPos(0), m_mouseyPos(0) {}

Renderer::~Renderer()
{
	glfwTerminate();
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

bool Renderer::vInitialize(std::string&& windowName, int width, int height,
                           std::function<void(float)>&& gameLogic)
{
	REQUIRE(!windowName.empty());
	REQUIRE(width >= 0);
	REQUIRE(height >= 0);
	if (windowName.empty()) {
		std::cout << "Missing window name" << std::endl;
		return false;
	}
	if (width < 0 || height < 0) {
		std::cout << "Invalid screen size" << std::endl;
		return false;
	}

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	m_gameLogic = gameLogic;

	// Init GLFW and set the required options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object to for GLFW's functions
	// TODO - read the size from config file
	m_window = glfwCreateWindow(width, height, std::move(windowName.c_str()), nullptr, nullptr);
	if (m_window == nullptr) {
		std::cerr << "\tFailed to create GLFW window" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE; // Uses more modern techniques for managing OpenGL functionality
	if (glewInit() != GLEW_OK) {
		// Initialize GLEW to setup the OpenGL Function pointers
		std::cerr << "\tFailed to initialize GLEW" << std::endl;
		return false;
	}
	int frameBufferWidth = 0;
	int frameBufferHeight = 0;
	glfwGetFramebufferSize(m_window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);

	// Set callback functions to static functions
	glfwSetFramebufferSizeCallback(m_window, Renderer::staticFramebufferSizeCallback); // Resize
	glfwSetKeyCallback(m_window, Renderer::staticKeyCallback); // Key
	glfwSetCursorPosCallback(m_window, Renderer::staticMouseCallback); // Mouse 
	// Cannot bind member functions to callbacks so bind window with pointer to this
	// With this we can call member functions from static callback functions
	glfwSetWindowUserPointer(m_window, this);

	#ifdef _DEBUG
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // For debugging use disabled cursor
	#else
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // For release do not let cursor out of the screen
	#endif
	glEnable(GL_DEPTH_TEST); // Enable depth testing

	// Create shader program by attaching and linking shaders to it
	m_shaderProgram = std::make_unique<ShaderProgram>();
	if (!m_shaderProgram->attachShader("vertex_basic.vert", GL_VERTEX_SHADER))
		return false;
	if (!m_shaderProgram->attachShader("fragment_basic.frag", GL_FRAGMENT_SHADER))
		return false;

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
	m_view = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -3.0f));
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
	std::cout << "\tOpenGL initialized succesfully" << std::endl;

	return true;
}

void Renderer::vStartMainLoop()
{
	REQUIRE(m_window != nullptr);
	REQUIRE(m_shaderProgram != nullptr);
	REQUIRE(m_shaderProgram->validate());
	if (m_window == nullptr || m_shaderProgram == nullptr || !m_shaderProgram->validate()) {
		std::cout << "OpenGL not properly initialized before calling vStartMainLoop" << std::endl;
		return;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture data
	std::unique_ptr<texture::Image> txrData = texture::load("square.bmp");
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

	std::cout << "Started main loop" << std::endl;
	float previousTick = utility::getTimestamp();
	while (!glfwWindowShouldClose(m_window)) {
		const float currentTick = utility::getTimestamp();
		const float deltaTime = currentTick - previousTick;

		glfwPollEvents();

		m_gameLogic(deltaTime);

		// Clear depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Clear color and set it to greenish
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind Texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		m_shaderProgram->use();

		// Set uniforms
		m_view = m_camera.getViewMatrix();
		m_shaderProgram->setMat4("view", m_view);
		m_shaderProgram->setMat4("projection", m_projection);

		// Render container
		glBindVertexArray(m_VAO);
		for (const auto cube : cubePositions) {
			m_model = glm::translate(glm::mat4(), cube);
			const float angle = 20.0f * static_cast<float>(glfwGetTime());
			m_model = glm::rotate(m_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			m_shaderProgram->setMat4("model", m_model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0); // Unbind
		glfwSwapBuffers(m_window);

		previousTick = currentTick;
	}

	std::cout << "Leaving from main loop" << std::endl;
}

void Renderer::staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	REQUIRE(window != nullptr);
	if (window == nullptr) {
		std::cout << "window parameter invalid in staticKeyCallback" << std::endl;
		return;
	}

	Renderer* r = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	r->keyCallback(key, scancode, action, mode);
}

void Renderer::keyCallback(int key, int scancode, int action, int mode) const
{
	REQUIRE(m_window != nullptr);
	if (m_window == nullptr) {
		std::cout << "OpenGL not properly initialized before calling keyCallback" << std::endl;
		return;
	}

	// Unused variables
	(void)mode;
	(void)scancode;

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

	// Handle events
	std::shared_ptr<IEvent> evt = nullptr;
	if (key == GLFW_KEY_W && action == GLFW_PRESS) { evt = std::make_shared<InputCommandEvent>("W"); }
	if (key == GLFW_KEY_A && action == GLFW_PRESS) { evt = std::make_shared<InputCommandEvent>("A"); }
	if (key == GLFW_KEY_S && action == GLFW_PRESS) { evt = std::make_shared<InputCommandEvent>("S"); }
	if (key == GLFW_KEY_D && action == GLFW_PRESS) { evt = std::make_shared<InputCommandEvent>("D"); }

	if (evt)
		EventManager::triggerEvent(evt);
}

void Renderer::staticMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	REQUIRE(window != nullptr);
	if (window == nullptr) {
		std::cout << "window parameter invalid in staticMouseCallback" << std::endl;
		return;
	}

	Renderer* r = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	r->mouseCallback(xpos, ypos);
}

void Renderer::mouseCallback(double xpos, double ypos)
{
	if (m_firstMouseMovement) {
		m_mousexPos = xpos;
		m_mouseyPos = ypos;
		m_firstMouseMovement = false;
	}
	const double xoffset = xpos - m_mousexPos;
	const double yoffset = m_mouseyPos - ypos; // Reversed since y-coordinates go from bottom to up
	m_mousexPos = xpos;
	m_mouseyPos = ypos;

	m_camera.processMouseMovement(xoffset, yoffset);
}

void Renderer::staticFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	REQUIRE(window != nullptr);
	REQUIRE(width >= 0);
	REQUIRE(height >= 0);
	if (window == nullptr) {
		std::cout << "window parameter invalid in staticFramebufferSizeCallback" << std::endl;
		return;
	}
	if (width < 0 || height < 0) {
		std::cout << "Invalid size " << std::endl;
		return;
	}

	Renderer* r = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	r->framebufferSizeCallback(width, height);
}

void Renderer::framebufferSizeCallback(int width, int height) const
{
	REQUIRE(m_window != nullptr);
	if (m_window == nullptr) {
		std::cout << "OpenGL not properly initialized before calling framebufferSizeCallback" << std::endl;
		return;
	}

	// Adjust viewport for window changes
	glViewport(0, 0, width, height);
	int framebufferWidth = 0, framebufferHeight = 0;
	glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);

	ENSURE(framebufferWidth == width);
	ENSURE(framebufferHeight == height);
}

glm::vec3 Renderer::vGetCameraFront() { return m_camera.getCameraFront(); }
