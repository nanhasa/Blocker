#include "Renderer/renderer.h"

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#pragma warning (pop)      // Restore back

#include "Event/eventmanager.h"
#include "Utility/contract.h"
#include "Utility/locator.h"
#include "Utility/utility.h"

Renderer::Renderer()
	: m_window(nullptr), m_width(0), m_height(0), m_sizeChanged(false), 
	m_shaderProgram(nullptr), m_log("Renderer") {}

Renderer::~Renderer()
{
	glfwTerminate();
}

bool Renderer::vInitialize(std::string&& windowName, std::function<void(float)>&& gameLogic)
{
	m_log.info("vInitialize", "Starting GLFW context, OpenGL 3.3");

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
		m_log.fatal("vInitialize", "Failed to create GLFW window");
		return false;
	}
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE; // Uses more modern techniques for managing OpenGL functionality
	if (glewInit() != GLEW_OK) {
		// Initialize GLEW to setup the OpenGL Function pointers
		m_log.fatal("vInitialize", "Failed to initialize GLEW");
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
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Cull back faces
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Create shader program by attaching and linking shaders to it
	m_shaderProgram = std::make_unique<ShaderProgram>();
	if (!m_shaderProgram->attachShader("vertex_basic.vert", GL_VERTEX_SHADER)) {
		m_log.fatal("vInitialize", "Could not attach shader: vertex_basic.vert");
		return false;
	}
	if (!m_shaderProgram->attachShader("fragment_basic.frag", GL_FRAGMENT_SHADER)) {
		m_log.fatal("vInitialize", "Could not attach shader: fragment_basic.frag");
		return false;
	}

	// Translate scene in the reverse direction of where we want to move
	m_projection = glm::perspective(glm::radians(45.0f),
	                                static_cast<float>(width / height), 0.1f, 100.0f);

	ENSURE(m_shaderProgram != nullptr);
	ENSURE(m_shaderProgram->validate());
	ENSURE(m_window != nullptr);
	m_log.info("vInitialize", "OpenGL initialized succesfully");

	return true;
}

void Renderer::vStartMainLoop()
{
	REQUIRE(m_window != nullptr);
	REQUIRE(m_shaderProgram != nullptr);
	REQUIRE(m_shaderProgram->validate());
	if (m_window == nullptr || m_shaderProgram == nullptr || !m_shaderProgram->validate()) {
		m_log.error("vStartMainLoop", "OpenGL not properly initialized before calling vStartMainLoop");
		return;
	}

	// Center cursor now that we are ready to start the game
	vCenterCursor();

	m_log.info("vStartMainLoop", "Started main loop");
	int previousTick = utility::timestampMs();
	while (!glfwWindowShouldClose(m_window)) {
		const int currentTick = utility::timestampMs();
		float const deltatime = static_cast<float>(currentTick - previousTick) / 1000;

		glfwPollEvents();

		// Clear depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Clear color and set it to greenish
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Set uniforms
		m_shaderProgram->setMat4("projection", m_projection);

		m_gameLogic(deltatime);

		glfwSwapBuffers(m_window);

		m_sizeChanged = false; // Reset sizeChanged variable

		previousTick = currentTick;
	}
	m_log.info("vStartMainLoop", "Leaving from main loop");
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

void Renderer::keyCallback(int key, int scancode, int action, int mode) const
{
	REQUIRE(m_window != nullptr);
	if (m_window == nullptr) {
		m_log.error("keyCallBack", "OpenGL not properly initialized before calling keyCallback");
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
		m_log.error("framebufferSizeCallback", "OpenGL not properly initialized before calling framebufferSizeCallback");
		return;
	}

	// Adjust viewport for window changes
	glViewport(0, 0, width, height);
	glfwGetFramebufferSize(m_window, &m_width, &m_height);
	m_sizeChanged = true;

	ENSURE(m_width == width);
	ENSURE(m_height == height);
}

ShaderProgram* Renderer::vGetShaderProgram() const
{
	return m_shaderProgram.get();
}

void Renderer::vGetCursorPosition(double& x, double& y) const
{
	REQUIRE(m_window);
	if (!m_window) {
		m_log.error("vGetCursorPosition", "OpenGL not properly initialized before calling vGetMousePosition");
		return;
	}
	glfwGetCursorPos(m_window, &x, &y);
}

void Renderer::vCenterCursor() const
{
	REQUIRE(m_window);
	if (!m_window) {
		m_log.error("vGetCursor", "OpenGL not properly initialized before calling vSetMousePosition");
		return;
	}
	glfwSetCursorPos(m_window, m_width / 2, m_height / 2);
}

bool Renderer::vKeyPressed(int key) const
{
	REQUIRE(m_window);
	if (!m_window) {
		m_log.error("vKeyPressed", "OpenGL not properly initialized before calling vGetKeyState");
		return false;
	}
	return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool Renderer::vWindowSizeChanged() const
{
	return m_sizeChanged;
}
