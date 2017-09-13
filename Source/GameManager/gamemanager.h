#pragma once

#include <memory>

#include "interfaces.h"
#include "Utility/logger.h"

class GameManager {
public:

	/**
	 * \brief Constructor. Call start to start the game logic
	 */
	GameManager();

	/**
	 * \brief Destructor
	 */
	~GameManager();

	/**
	 * \brief Initialize game and start main loop
	 * \return True if initializations were successful, otherwise false
	 */
	bool start();

	/**
	 * \brief Function called on every frame to update game logic
	 * \param deltatime Time in seconds since last main loop started
	 */
	void onUpdate(float deltatime);

private:
	std::unique_ptr<IRenderer> m_renderer;	//!< Pointer to renderer component
	Logger m_log;							//!< Logger object used to write log entries
};
