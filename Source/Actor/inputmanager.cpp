#include "Actor/inputmanager.h"
#include "Actor/player.h"

#include <tuple>

InputManager::InputManager() 
	: m_tempPosition(), m_mouseSensitivity(0.1f), m_speedMultiplier(2.0f), 
	m_firstMouseMovement(true), m_mousexPos(0), m_mouseyPos(0) {}

InputManager::~InputManager() {}

void InputManager::onUpdate(Player & player, IRenderer & renderer, Camera & camera, float deltatime)
{
	updateRotation(player, renderer);
	updatePosition(player, renderer, camera, deltatime);
}

void InputManager::updateRotation(Player& player, IRenderer& renderer)
{
	auto mousePos = renderer.vGetMousePosition();
	auto xpos = std::get<0>(mousePos);
	auto ypos = std::get<1>(mousePos);

	// Handle the first mouse movement
	if (m_firstMouseMovement) {
		m_mousexPos = xpos;
		m_mouseyPos = ypos;
		m_firstMouseMovement = false;
	}

	// Reverse since y-coordinates go from bottom to up
	auto xOffset = xpos - m_mousexPos;
	auto yOffset = m_mouseyPos - ypos;
	m_mousexPos = xpos;
	m_mouseyPos = ypos;

	//Check if it is necessary to update dirtyFlag
	if (xOffset == 0 && yOffset == 0)
		return;

	xOffset *= m_mouseSensitivity;
	yOffset *= m_mouseSensitivity;

	player.transform.rotation.x += static_cast<float>(yOffset);
	player.transform.rotation.y += static_cast<float>(xOffset);

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
