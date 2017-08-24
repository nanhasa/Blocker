#pragma once

#include <memory>

#include "interfaces.h"

class GameManager {
public:

	/**
	 * \brief Builder. Call start to start the game logic
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

	/**
	 * \brief Function used to test event system delegates. Will be deleted soon.
	 * \param eventData Pointer to the event data object
	 */
	void testDelegate(std::shared_ptr<IEvent> eventData);

private:
	std::unique_ptr<IRenderer> m_renderer; //!< Pointer to renderer component
};
