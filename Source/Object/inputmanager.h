#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "interfaces.h"
#include "Utility/logger.h"

class Player; // Forward declaration to avoid circular includes

class InputManager {
public:

	/**
	 * \brief Constructor
	 */
	InputManager();

	/**
	 * \brief Destructor
	 */
	~InputManager();

	InputManager(InputManager const&) = delete;
	InputManager& operator=(const InputManager& rhs) = delete;

	/**
	 * \brief operator=
	 * \param rhs
	 * \return 
	 */
	InputManager& operator=(InputManager&& rhs) noexcept;

	/**
	 * \brief Used to handle input on each frame
	 * \param player Parent object to be updated
	 * \param renderer Renderer used to access input
	 * \param deltatime Time in seconds since last frame
	 */
	void onUpdate(Player& player, IRenderer& renderer, const float deltatime);

private:
	glm::vec3 m_tempPosition;		//!< Temp for new position to be checked before assigning it

	float m_mouseSensitivity;		//!< Multiplier used to make camera movement more or less sensitive
	float m_speedMultiplier;		//!< Multiplier used to make player movement more or less fast
	double m_lastCursorxPos;		//!< Last recorded mouse position on x-axis
	double m_lastCursoryPos;		//!< Last recorded mouse position on y-axis

	Logger m_log;

	/**
	 * \brief Used to update parent object's rotation based on cursor movement
	 * \param player Parent object to be updated
	 * \param renderer Renderer used to access mouse movement
	 */
	void updateRotation(Player& player, IRenderer& renderer);

	/**
	 * \brief Used to update parent object's position based on key input
	 * \param player Parent object to be updated
	 * \param renderer Renderer used to access key states
	 * \param deltatime Time in seconds since last frame. Used to even out movement speed
	 */
	void updatePosition(Player& player, IRenderer& renderer, const float deltatime);
};
