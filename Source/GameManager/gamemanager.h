#pragma once

#include <memory>

#include "interfaces.h"
#include "GameManager/worldmanager.h"
#include "Object/player.h"
#include "Utility/logger.h"

class GameManager {
public:

	/**
	 * \brief Constructor. Call start to start the game logic
	 */
	GameManager();

	~GameManager() = default;

	/**
	 * \brief Destructor
	 * \return True if initializations were successful, otherwise false
	 */
	bool start();

	/**
	 * \brief Function called on every frame to update game logic
	 * \param deltatime Time in seconds since last main loop started
	 */
	void onUpdate(const float deltatime);

private:
	std::unique_ptr<IRenderer> m_renderer;	//!< Pointer to renderer component
	Player m_player;						//!< Player
	Logger m_log;							//!< Logger object used to write log entries
	std::unique_ptr<WorldManager> m_world;	//!< Holds data of objects in the world
};
