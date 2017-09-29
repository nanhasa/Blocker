#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "interfaces.h"
#include "Renderer/camera.h"

class Player;

class InputManager {
public:

	/**
	 * \brief InputManager
	 */
	InputManager();

	/**
	 * \brief ~InputManager
	 */
	~InputManager();

	/**
	 * \brief onUpdate
	 * \param player
	 * \param renderer
	 * \param camera
	 * \param deltatime
	 */
	void onUpdate(Player& player, IRenderer& renderer, Camera& camera, float deltatime);

private:
	glm::vec3 m_tempPosition;		//!< Temp for new position to be checked before assigning it

	const float m_mouseSensitivity;	//!< Multiplier used to make camera movement more or less sensitive
	const float m_speedMultiplier;	//!< Multiplier used to make player movement more or less fast
	double m_lastCursorxPos;		//!< Last recorded mouse position on x-axis
	double m_lastCursoryPos;		//!< Last recorded mouse position on y-axis

	/**
	 * \brief updateRotation
	 * \param player
	 * \param renderer
	 */
	void updateRotation(Player& player, IRenderer& renderer);

	/**
	 * \brief updatePosition
	 * \param player
	 * \param renderer
	 * \param camera
	 * \param deltatime
	 */
	void updatePosition(Player& player, IRenderer& renderer, Camera& camera, float deltatime);
};
