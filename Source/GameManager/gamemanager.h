#pragma once

#include <memory>

class Renderer;

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
	std::unique_ptr<Renderer> m_renderer;
};
