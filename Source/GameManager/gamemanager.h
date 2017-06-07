#pragma once

#include <memory>

#include "interfaces.h"

class GameManager {
public:

	/**
	 * \brief GameManager
	 */
	GameManager();

	/**
	 * \brief ~GameManager
	 */
	~GameManager();

	/**
	 * \brief Initialize game and start main loop
	 * \return Operation successful
	 */
	bool start();

	/**
	 * \brief Function called on every frame to update game logic
	 */
	void onUpdate();

private:
	std::unique_ptr<IRenderer> m_renderer;
};
