#include "Actor/inputmanager.h"
#include "Actor/player.h"

InputManager::InputManager(int screenWidth, int screenHeight)
	: m_tempPosition(), m_mouseSensitivity(0.1f), m_speedMultiplier(2.0f), 
	m_centerxPos(screenWidth / 2), m_centeryPos(screenHeight / 2) {}

InputManager::~InputManager() {}

void InputManager::onUpdate(Player & player, IRenderer & renderer, Camera & camera, float deltatime)
{
	updateRotation(player, renderer);
	updatePosition(player, renderer, camera, deltatime);
}

void InputManager::updateRotation(Player& player, IRenderer& renderer)
{
	double xpos = 0.0;
	double ypos = 0.0;
	renderer.vGetCursorPosition(xpos, ypos); // Save mouse position to reference parameters
	renderer.vCenterCursor();

	// Reverse since y-coordinates go from bottom to up
	auto xOffset = xpos - m_centerxPos;
	auto yOffset = m_centeryPos - ypos;

	//Check if it is necessary to update dirtyFlag
	if (xOffset == 0 && yOffset == 0)
		return;

	xOffset *= m_mouseSensitivity;
	yOffset *= m_mouseSensitivity;

	player.transform.rotation.x += static_cast<float>(yOffset);
	player.transform.rotation.y += static_cast<float>(xOffset);

	// Make yaw continuous between 0..360
	if (player.transform.rotation.y < 0)
		player.transform.rotation.y += 360;
	if (player.transform.rotation.y > 360)
		player.transform.rotation.y -= 360;

	// Limit pitch value
	if (player.transform.rotation.x > 89.0)
		player.transform.rotation.x = 89.0;
	if (player.transform.rotation.x < -89.0)
		player.transform.rotation.x = -89.0;
}

void InputManager::updatePosition(Player& player, IRenderer& renderer, Camera& camera, float deltatime)
{
	m_tempPosition = player.transform.position;
	if (renderer.vKeyPressed(static_cast<int>('W')))
		m_tempPosition += camera.getCameraFront() * deltatime * m_speedMultiplier;
	if (renderer.vKeyPressed(static_cast<int>('A')))
		m_tempPosition -= camera.getCameraRight() * deltatime * m_speedMultiplier;
	if (renderer.vKeyPressed(static_cast<int>('S')))
		m_tempPosition -= camera.getCameraFront() * deltatime * m_speedMultiplier;
	if (renderer.vKeyPressed(static_cast<int>('D')))
		m_tempPosition += camera.getCameraRight() * deltatime * m_speedMultiplier;

	// Test if tempPosition is legit
	// TODO: actually test it

	player.transform.position = m_tempPosition;
}
