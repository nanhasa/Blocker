#pragma once

#include <vector>

#include "Object/player.h"
#include "Object/terrain.h"
#include "Renderer/modelmanager.h"

class WorldManager {
public:

	/**
	 * \brief WorldManager
	 */
	WorldManager();
	~WorldManager() = default;

	/**
	 * \brief onUpdate
	 * \param player
	 * \param renderer
	 * \param deltatime
	 */
	void onUpdate(Player& player, IRenderer& renderer, const float deltatime);

private:
	std::vector<std::unique_ptr<Terrain>> m_objects;	//!< Objects in 3d world
	ModelManager m_modelManager;						//!< Used to get references to textures and models
};
