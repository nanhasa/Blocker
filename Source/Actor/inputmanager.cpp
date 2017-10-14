#include "Actor/inputmanager.h"
#include "Actor/player.h"
#include "Utility/contract.h"
#include "Utility/locator.h"

InputManager::InputManager()
	: m_tempPosition(), m_lastCursorxPos(0), m_lastCursoryPos(0), m_log("InputManager")
{
	m_mouseSensitivity = Locator::getConfig()->get("MouseSensitivity", 0.1f);
	m_speedMultiplier = Locator::getConfig()->get("MovementSpeedMultiplier", 2.0f);
}

InputManager::~InputManager() {}

InputManager & InputManager::operator=(InputManager && rhs) noexcept
{
	m_tempPosition = std::move(rhs.m_tempPosition);
	m_mouseSensitivity = std::move(rhs.m_mouseSensitivity);
	m_speedMultiplier = std::move(rhs.m_speedMultiplier);
	m_lastCursorxPos = std::move(rhs.m_lastCursorxPos);
	m_lastCursoryPos = std::move(rhs.m_lastCursoryPos);
	m_log = std::move(m_log);
	return *this;
}

void InputManager::onUpdate(Player& player, IRenderer* renderer, float deltatime)
{
	REQUIRE(renderer);
	if (!renderer) {
		m_log.error("No pointer to renderer was provided in onUpdate");
		return;
	}
	updateRotation(player, *renderer);
	updatePosition(player, *renderer, deltatime);
}

void InputManager::updateRotation(Player& player, IRenderer& renderer)
{
	// Don't update rotation if screen size changed as it messes up the offset logic
	if (renderer.vWindowSizeChanged()) {
		renderer.vCenterCursor();
		return;
	}

	// Save mouse position to reference parameters
	double xpos = 0.0;
	double ypos = 0.0;
	renderer.vGetCursorPosition(xpos, ypos);
	renderer.vCenterCursor();
	renderer.vGetCursorPosition(m_lastCursorxPos, m_lastCursoryPos);

	auto xOffset = m_lastCursorxPos - xpos;
	auto yOffset = m_lastCursoryPos - ypos;
	
	//Check if it is necessary to update rotation
	if (xOffset == 0 && yOffset == 0)
		return;
	
	// Add sensitivity factor
	xOffset *= m_mouseSensitivity;
	yOffset *= m_mouseSensitivity;
	
	// Rotate player, no need to use temp as rotation should be allowed always
	player.transform.rotation.x += static_cast<float>(yOffset);
	player.transform.rotation.y += static_cast<float>(xOffset);

	const auto y = player.transform.rotation.y;
	const auto x = player.transform.rotation.x;
	// Make yaw continuous between 0..360 (looking left increases number)
	if (y < 0)
		player.transform.rotation.y += 360;
	if (y > 360)
		player.transform.rotation.y -= 360;

	// Make pitch continuous between 0..360 (Looking up increases number)
	if (y < 0)
		player.transform.rotation.y += 360;
	if (y > 360)
		player.transform.rotation.y -= 360;
	
	// Limit pitch value between 271..89 (271 looks down, 89 looks up)
	if (x > 89 && x < 271) {
		if (glm::abs(x - 89) < glm::abs(x - 271))
			player.transform.rotation.x = 89;
		else
			player.transform.rotation.x = 271;
	}
}

void InputManager::updatePosition(Player& player, IRenderer& renderer, float deltatime)
{
	m_tempPosition = player.transform.position;
	const auto multiplier = deltatime * m_speedMultiplier;
	if (renderer.vKeyPressed(static_cast<int>('W')))
		m_tempPosition += player.transform.getDirectionForward() * multiplier;
	if (renderer.vKeyPressed(static_cast<int>('A')))
		m_tempPosition -= player.transform.getDirectionRight() * multiplier;
	if (renderer.vKeyPressed(static_cast<int>('S')))
		m_tempPosition -= player.transform.getDirectionForward() * multiplier;
	if (renderer.vKeyPressed(static_cast<int>('D')))
		m_tempPosition += player.transform.getDirectionRight() * multiplier;

	// Test if tempPosition is legit
	// TODO: actually test it

	player.transform.position = std::move(m_tempPosition);
}
