#pragma once

#include <vector>

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
	 * \param renderer
	 * \param deltatime
	 */
	void onUpdate(IRenderer& renderer, const float deltatime);

private:
	std::vector<Terrain> m_objects;	//!< Objects in 3d world
	ModelManager m_modelManager;	//!< Used to get references to textures and models
};
